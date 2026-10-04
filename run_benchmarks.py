#!/usr/bin/env python3
"""CHEHAB benchmark runner for BFV and CKKS benchmarks.

Every benchmark goes through the same two phases: the CHEHAB compile phase
(DSL -> Compiler -> generated SEAL code in build/benchmarks/<dir>/he) and the
execution phase (build the generated program, run it). Each run configuration
produces one result record with the same fields for both schemes; fields that
do not apply to a scheme are left empty. See docs/benchmark_results.md.

Examples:
  python run_benchmarks.py                                    # BFV sweep, as before
  python run_benchmarks.py --scheme ckks                      # all CKKS benchmarks
  python run_benchmarks.py --scheme all --vectorize 0 --slot-counts 4 \\
      --benchmarks nn_linear nn_conv2d nn_avgpool2d quad_ckks \\
      --markdown docs/unified_baseline.md

Timing: the compile phase runs --compile-runs times (median reported). The
generated program runs --warmup times unmeasured, then --repeats measured
times; the median of the measured end-to-end execution times (CHEHAB
convention: parse inputs + encode + encrypt + evaluate) is the primary
statistic. CKKS programs also report an evaluation-only median (fhe() alone,
1 warmup + --eval-repeats runs inside each process). All times are in ms.
BFV and CKKS use different encryption parameters; do not compare their
latencies with each other.
"""
import argparse
import csv
import json
import os
import platform
import re
import shutil
import statistics
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, Dict, List, Optional

REPO = Path(__file__).resolve().parent
# The BFV benchmark CMake files copy their runtime into <repo>/build.
BUILD = REPO / "build"

# ---------------------------------------------------------------------------
# Benchmark registry
# ---------------------------------------------------------------------------
BFV_BENCHMARKS = ["max", "sort", "box_blur", "lin_reg", "hamming_dist", "poly_reg", "l2_distance", "dot_product",
                  "gx_kernel", "gy_kernel", "roberts_cross", "matrix_mul", "nn_linear", "nn_conv2d", "nn_avgpool2d"]
BFV_DEFAULT_SLOT_COUNTS = [3, 4, 5, 8, 16, 32]
BFV_FIXED_SLOT_COUNTS = {"max": [3, 4, 5], "sort": [3, 4], "discrete_cosin_transform": [1], "poly_derivative": [1]}
BFV_WITHOUT_GENERATOR = {"max", "sort", "discrete_cosin_transform", "poly_derivative"}
POLYNOMIAL_DEPTHS = [5, 10]
POLYNOMIAL_REGIMES = ["50-50", "100-50", "100-100"]
POLYNOMIAL_INSTANCES = 1

ACTIVATIONS = ["gelu", "silu", "sigmoid", "elu", "selu", "softplus", "mish", "hardshrink", "relu"]


@dataclass
class Case:
    """One run configuration of a benchmark."""
    name: str
    # Prepares inputs in the benchmark build dir; returns (slot_count, info).
    prepare: Callable[[Path], tuple]
    # Compile-phase command, given the slot count.
    compile_cmd: Callable[[Optional[int]], List[str]]
    # CKKS: levels the program must consume (None: taken from the generator, or not checked).
    expected_levels: Optional[int] = None


@dataclass
class Benchmark:
    name: str
    scheme: str  # "BFV" or "CKKS"
    directory: str
    cases: List[Case] = field(default_factory=list)
    path: str = ""  # compiler path description


def run(cmd, cwd, timeout=None, check=True, env=None):
    result = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                            timeout=timeout, env=env)
    if check and result.returncode != 0:
        raise RuntimeError(f"`{' '.join(map(str, cmd))}` failed in {cwd} (exit {result.returncode}):\n{result.stdout}")
    return result


def bfv_benchmarks(args) -> List[Benchmark]:
    path = (f"vectorized ({'RL' if args.opt_method == 1 else 'e-graph'}, window {args.window})"
            if args.vectorize else "scalar (simplification_ruleset)")
    benches = []
    for name in BFV_BENCHMARKS:
        slots = BFV_FIXED_SLOT_COUNTS.get(name, args.slot_counts or BFV_DEFAULT_SLOT_COUNTS)
        cases = []
        for slot in slots:
            def prepare(bench_dir, name=name, slot=slot):
                if name not in BFV_WITHOUT_GENERATOR:
                    run(["python3", f"generate_{name}.py", "--slot_count", str(slot)], bench_dir)
                return slot, {}

            def compile_cmd(slot, name=name):
                return [f"./{name}", str(args.vectorize), str(slot), str(args.opt_method), str(args.window), "1",
                        str(args.cse), str(args.const_folding)]

            cases.append(Case(str(slot), prepare, compile_cmd))
        benches.append(Benchmark(name, "BFV", name, cases, path))

    poly_cases = []
    for regime in POLYNOMIAL_REGIMES:
        for depth in POLYNOMIAL_DEPTHS:
            for instance in range(1, POLYNOMIAL_INSTANCES + 1):
                def compile_cmd(slot, regime=regime, depth=depth, instance=instance):
                    return ["./polynomials_coyote", str(depth), str(instance), regime, str(args.vectorize),
                            str(args.opt_method)]

                poly_cases.append(Case(f"tree_{regime}_{depth}_{instance}", lambda d: (None, {}), compile_cmd))
    benches.append(Benchmark("polynomials_coyote", "BFV", "polynomials_coyote", poly_cases, path))
    return benches


def ckks_benchmarks(args) -> List[Benchmark]:
    """CKKS benchmarks always run on the scalar path (CKKS functions reject vectorization)."""
    path = "scalar (simplification_ruleset)"
    tail = [str(args.cse), str(args.const_folding)]

    def first_line_slot(cmd):
        def prepare(bench_dir):
            out = run(cmd, bench_dir).stdout
            return int(out.splitlines()[0]), {"generator_output": out}
        return prepare

    quad = Benchmark("quad_ckks", "CKKS", "quad_ckks", path=path)
    for case, slot, seed in (("fixed", 8, 0), ("dense", 256, 1)):
        def prepare(bench_dir, slot=slot, seed=seed):
            run(["python3", "generate_quad_ckks.py", "--slot_count", str(slot), "--seed", str(seed)], bench_dir)
            return slot, {}
        quad.cases.append(Case(case, prepare, lambda s: ["./quad_ckks", str(s), "1", *tail], 1))

    benches = [quad]
    # Elementwise add (no level) and multiply (one level) of a ciphertext and a
    # ciphertext, a plaintext vector (_plain) or a plaintext scalar (_scalar).
    for op, levels in (("add", 0), ("add_plain", 0), ("add_scalar", 0),
                       ("mul", 1), ("mul_plain", 1), ("mul_scalar", 1)):
        bench = Benchmark(f"{op}_ckks", "CKKS", "elementwise_ckks", path=path)
        for case, slot, seed in (("fixed", 8, 0), ("dense", 256, 1)):
            def prepare(bench_dir, op=op, slot=slot, seed=seed):
                run(["python3", "generate_elementwise_ckks.py", "--op", op, "--slot_count", str(slot), "--seed",
                     str(seed)], bench_dir)
                return slot, {}
            bench.cases.append(Case(case, prepare, lambda s, op=op: ["./elementwise_ckks", op, str(s), "1", *tail],
                                    levels))
        benches.append(bench)

    # ReLU through a composite minimax sign approximation (three degree-7 stages).
    relu = Benchmark("relu_ckks", "CKKS", "relu_ckks", path=path)
    for case, points in (("fixed", "points9"), ("dense", "dense")):
        relu.cases.append(Case(case, first_line_slot(["python3", "generate_relu_ckks.py", points]),
                               lambda s: ["./relu_ckks", str(s), "1", *tail]))
    benches.append(relu)

    for name, fixed, dense in (("batchnorm1d_ckks", "bn1d", "bn1d_rand"), ("batchnorm2d_ckks", "bn2d", "bn2d_rand")):
        bench = Benchmark(name, "CKKS", "batchnorm_ckks", path=path)
        for case, generator_case in (("fixed", fixed), ("dense", dense)):
            bench.cases.append(Case(case, first_line_slot(["./generate_batchnorm_ckks", generator_case]),
                                    lambda s: ["./batchnorm_ckks", str(s), "1", *tail], 1))
        benches.append(bench)

    for activation, degree in [(a, 7) for a in ACTIVATIONS] + [("sigmoid", 5), ("relu", 2)]:
        bench = Benchmark(f"chebyshev_{activation}_d{degree}_ckks", "CKKS", "chebyshev_ckks", path=path)
        for case, points in (("fixed", "points9"), ("dense", "dense")):
            bench.cases.append(Case(
                case,
                first_line_slot(["./generate_chebyshev_ckks", "p", points, activation, str(degree)]),
                lambda s, a=activation, d=degree: ["./chebyshev_ckks", "p", str(s), "1", *tail, a, str(d)]))
        benches.append(bench)
    return benches


# ---------------------------------------------------------------------------
# Result record
# ---------------------------------------------------------------------------
OP_FIELDS = {
    # evaluator method in generated code -> record field
    "add": "op_add", "add_plain": "op_add_plain", "sub": "op_sub", "sub_plain": "op_sub_plain",
    "multiply": "op_multiply", "multiply_plain": "op_multiply_plain", "square": "op_square",
    "relinearize": "op_relinearize", "rescale_to_next": "op_rescale", "mod_switch_to_next": "op_mod_switch",
    "mod_switch_to": "op_mod_switch", "rotate_rows": "op_rotate", "rotate_columns": "op_rotate",
    "rotate_vector": "op_rotate", "negate": "op_negate",
}

RECORD_FIELDS = [
    "record_id", "benchmark", "case", "scheme", "status", "error",
    "poly_modulus_degree", "coeff_modulus", "plain_modulus_bits", "log2_scale", "slot_count",
    "compiler_path", "cse", "const_folding",
    "compile_time_ms", "compile_runs",
    "exec_time_ms", "exec_time_min_ms", "exec_time_max_ms", "exec_warmup_runs", "exec_measured_runs",
    "eval_time_ms", "eval_runs_per_process",
    "depth", "xdepth",
    "op_add", "op_add_plain", "op_sub", "op_sub_plain", "op_multiply", "op_multiply_plain", "op_square",
    "op_relinearize", "op_rescale", "op_mod_switch", "op_rotate", "op_negate",
    "bfv_outputs_match", "bfv_noise_budget_bits",
    "ckks_fhe_max_abs_error", "ckks_tolerance", "ckks_chain_index_start", "ckks_chain_index_end",
    "ckks_levels_consumed", "ckks_expected_levels", "ckks_output_log2_scale",
    "ckks_approx_error_test_points", "ckks_max_fit_error",
]


def count_ops(source: str) -> Dict[str, int]:
    counts = {f: 0 for f in set(OP_FIELDS.values())}
    for method in re.findall(r"\bevaluator\.(\w+)\(", source):
        if method in OP_FIELDS:
            counts[OP_FIELDS[method]] += 1
    return counts


def parse_params(main_source: str) -> Dict[str, object]:
    params = {}
    if m := re.search(r"size_t n\s*=\s*(\d+)", main_source):
        params["poly_modulus_degree"] = int(m.group(1))
    if m := re.search(r"set_coeff_modulus\((CoeffModulus::\w+\([^;]*)\);", main_source):
        params["coeff_modulus"] = re.sub(r"\s+", " ", m.group(1))
    if m := re.search(r"PlainModulus::Batching\(n,\s*(\d+)\)", main_source):
        params["plain_modulus_bits"] = int(m.group(1))
    if m := re.search(r"scale = pow\(2\.0,\s*(\d+)\)", main_source):
        params["log2_scale"] = int(m.group(1))
    return params


def compile_phase(bench_dir: Path, cmd: List[str], runs: int, timeout: float):
    times, out = [], ""
    for _ in range(runs):
        out = run(cmd, bench_dir, timeout=timeout).stdout
        m = re.search(r"^([0-9.eE+-]+) ms$", out, re.M)
        if not m:
            raise RuntimeError(f"no compile time in the output of {' '.join(cmd)}:\n{out}")
        times.append(float(m.group(1)))
    depth = re.search(r"max:\s*\((\d+),\s*(\d+)\)", out)
    return times, ((int(depth.group(1)), int(depth.group(2))) if depth else (None, None))


def build_generated(bench_dir: Path, prefix: str, env):
    he = bench_dir / "he"
    shutil.rmtree(he / "build", ignore_errors=True)
    run(["cmake", "-S", ".", "-B", "build", f"-DCMAKE_PREFIX_PATH={prefix}", "-DCMAKE_BUILD_TYPE=Release"], he,
        env=env)
    run(["cmake", "--build", "build", "-j8"], he, env=env)


def execute(bench_dir: Path, scheme: str, args) -> List[subprocess.CompletedProcess]:
    cmd = ["./main"]
    if scheme == "CKKS":
        cmd += ["--tol", str(args.ckks_tolerance), "--repeats", str(args.eval_repeats)]
    results = []
    for i in range(args.warmup + args.repeats):
        result = run(cmd, bench_dir / "he" / "build", check=False, timeout=args.exec_timeout)
        if i >= args.warmup:
            results.append(result)
    return results


def floats(pattern: str, outputs: List[str]) -> List[float]:
    return [float(v) for out in outputs for v in re.findall(pattern, out)]


def run_case(bench: Benchmark, case: Case, args, env) -> Dict[str, object]:
    record: Dict[str, object] = {
        "record_id": f"{bench.name}_{case.name}", "benchmark": bench.name, "case": case.name,
        "scheme": bench.scheme, "compiler_path": bench.path, "cse": args.cse, "const_folding": args.const_folding,
        "compile_runs": args.compile_runs, "exec_warmup_runs": args.warmup, "exec_measured_runs": args.repeats,
    }
    bench_dir = BUILD / "benchmarks" / bench.directory
    try:
        slot, info = case.prepare(bench_dir)
        record["slot_count"] = slot if slot is not None else ""
        compile_times, (depth, xdepth) = compile_phase(bench_dir, case.compile_cmd(slot), args.compile_runs,
                                                       args.compile_timeout)
        record.update(compile_time_ms=statistics.median(compile_times), depth=depth, xdepth=xdepth)
        he = bench_dir / "he"
        record.update(count_ops((he / "_gen_he_fhe.cpp").read_text()))
        record.update(parse_params((he / "main.cpp").read_text()))

        build_generated(bench_dir, args.seal_prefix, env)
        results = execute(bench_dir, bench.scheme, args)
        outputs = [r.stdout for r in results]
        exec_times = floats(r"execution_time_\(ms\):\s*([0-9.eE+-]+)", outputs)
        if len(exec_times) != len(results):
            raise RuntimeError("generated program did not report execution_time_(ms):\n" + outputs[-1])
        record.update(exec_time_ms=statistics.median(exec_times), exec_time_min_ms=min(exec_times),
                      exec_time_max_ms=max(exec_times))

        if bench.scheme == "BFV":
            crashed = [r for r in results if r.returncode != 0]
            if crashed:
                raise RuntimeError(f"generated program exited {crashed[0].returncode}:\n{crashed[0].stdout}")
            matches = [int(v) for v in floats(r"outputs_match:\s*(\d)", outputs)]
            noise = floats(r"Remaining_noise_budget:\s*(\d+)", outputs)
            record["bfv_noise_budget_bits"] = int(min(noise)) if noise else ""
            if not matches:
                record.update(bfv_outputs_match="", status="NOT_CHECKED")
            else:
                ok = all(matches) and len(matches) == len(results)
                record.update(bfv_outputs_match=int(ok), status="PASS" if ok else "FAIL")
        else:
            errors = floats(r"max_abs_error=([0-9.eE+-]+)", outputs)
            evals = floats(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", outputs)
            chain = re.findall(r"chain_index (\d+) -> (\d+) \(levels consumed (\d+)\)", outputs[-1])
            scale = re.findall(r"log2_scale=([0-9.eE+-]+)", outputs[-1])
            gen_out = info.get("generator_output", "")
            expected = case.expected_levels
            if expected is None and (m := re.search(r"reference levels: (\d+)", gen_out)):
                expected = int(m.group(1))
            record.update(
                eval_time_ms=statistics.median(evals) if evals else "", eval_runs_per_process=args.eval_repeats,
                ckks_fhe_max_abs_error=max(errors) if errors else "", ckks_tolerance=args.ckks_tolerance,
                ckks_expected_levels=expected if expected is not None else "",
                ckks_output_log2_scale=float(scale[0]) if scale else "")
            if chain:
                start, end, levels = map(int, chain[0])
                record.update(ckks_chain_index_start=start, ckks_chain_index_end=end, ckks_levels_consumed=levels)
            if m := re.search(r"on these points: ([0-9.eE+-]+)", gen_out):
                record["ckks_approx_error_test_points"] = float(m.group(1))
            if m := re.search(r"max_fit_error \(fitting grid[^)]*\): ([0-9.eE+-]+)", gen_out):
                record["ckks_max_fit_error"] = float(m.group(1))
            ok = (all(r.returncode == 0 for r in results) and bool(chain)
                  and (expected is None or record.get("ckks_levels_consumed") == expected))
            record["status"] = "PASS" if ok else "FAIL"
    except (RuntimeError, subprocess.TimeoutExpired, OSError) as e:
        record.update(status="ERROR", error=str(e).splitlines()[0][:300])
        print(str(e), file=sys.stderr)
    return record


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------
def fmt(value, spec=""):
    if value == "" or value is None:
        return "-"
    return format(value, spec) if spec else str(value)


def markdown(records: List[Dict[str, object]], meta: Dict[str, object]) -> str:
    lines = [
        "# Unified CHEHAB benchmark baseline", "",
        f"Generated by `run_benchmarks.py` at commit {meta['commit']}{' (with local changes)' if meta['dirty'] else ''} "
        f"on {meta['platform']}, {meta['compiler']}, Microsoft SEAL {meta['seal']}.", "",
        f"Command: `{meta['command']}`", "",
        f"Timing: compile phase median of {meta['compile_runs']} runs; generated program {meta['warmup']} warmup "
        f"+ {meta['repeats']} measured runs, median (min-max) of CHEHAB's end-to-end execution time (parse inputs + "
        "encode + encrypt + evaluate). CKKS also reports an evaluation-only median (fhe() alone, 1 warmup + "
        f"{meta['eval_repeats']} runs per process, median over the measured processes). All times in ms.", "",
        "BFV and CKKS rows use different encryption parameters (see the parameters column): **do not compare "
        "latency across the two tables.**", "",
    ]
    ops_common = [("add", "op_add"), ("add_p", "op_add_plain"), ("sub", "op_sub"), ("sub_p", "op_sub_plain"),
                  ("mul", "op_multiply"), ("mul_p", "op_multiply_plain"), ("relin", "op_relinearize")]

    def params(r):
        parts = [f"N={fmt(r.get('poly_modulus_degree'))}"]
        if r["scheme"] == "BFV":
            parts.append(f"t={fmt(r.get('plain_modulus_bits'))} bits")
            parts.append(str(r.get("coeff_modulus", "")).replace("CoeffModulus::", ""))
        else:
            parts.append(f"scale=2^{fmt(r.get('log2_scale'))}")
            parts.append(str(r.get("coeff_modulus", "")).replace("CoeffModulus::Create(n, ", "q=").rstrip(")"))
        return ", ".join(parts)

    bfv = [r for r in records if r["scheme"] == "BFV"]
    ckks = [r for r in records if r["scheme"] == "CKKS"]
    if bfv:
        ops = ops_common + [("rot", "op_rotate"), ("neg", "op_negate")]
        lines += ["## BFV", "",
                  "| Benchmark | Slots | Status | Outputs match | Parameters | Path | Compile (ms) | "
                  "Exec median (min-max) | (depth, xdepth) | " + " | ".join(o for o, _ in ops) +
                  " | Noise budget (bits) |",
                  "|" + "---|" * (10 + len(ops))]
        for r in bfv:
            lines.append(
                f"| {r['benchmark']} | {r['case']} | {r['status']} | {fmt(r.get('bfv_outputs_match'))} | {params(r)} | "
                f"{r['compiler_path']} | {fmt(r.get('compile_time_ms'), '.2f')} | "
                f"{fmt(r.get('exec_time_ms'), '.1f')} ({fmt(r.get('exec_time_min_ms'), '.1f')}-"
                f"{fmt(r.get('exec_time_max_ms'), '.1f')}) | ({fmt(r.get('depth'))}, {fmt(r.get('xdepth'))}) | " +
                " | ".join(fmt(r.get(f)) for _, f in ops) + f" | {fmt(r.get('bfv_noise_budget_bits'))} |")
        lines += ["", "BFV-specific: **outputs match** compares every decrypted output with the expected values "
                  "(exact) in every measured run; **noise budget** is the smallest remaining invariant noise budget "
                  "over outputs and runs.", ""]
    if ckks:
        ops = ops_common + [("rescale", "op_rescale"), ("mod_sw", "op_mod_switch")]
        lines += ["## CKKS", "",
                  "| Benchmark | Case (slots) | Status | Parameters | Compile (ms) | Exec median (min-max) | "
                  "Eval-only median | (depth, xdepth) | " + " | ".join(o for o, _ in ops) +
                  " | FHE max abs error | max_fit_error | Approx. error (test pts) | Chain | Levels |",
                  "|" + "---|" * (13 + len(ops))]
        for r in ckks:
            lines.append(
                f"| {r['benchmark']} | {r['case']} ({fmt(r.get('slot_count'))}) | {r['status']} | {params(r)} | "
                f"{fmt(r.get('compile_time_ms'), '.2f')} | {fmt(r.get('exec_time_ms'), '.1f')} "
                f"({fmt(r.get('exec_time_min_ms'), '.1f')}-{fmt(r.get('exec_time_max_ms'), '.1f')}) | "
                f"{fmt(r.get('eval_time_ms'), '.1f')} | ({fmt(r.get('depth'))}, {fmt(r.get('xdepth'))}) | " +
                " | ".join(fmt(r.get(f)) for _, f in ops) +
                f" | {fmt(r.get('ckks_fhe_max_abs_error'), '.2e')} | {fmt(r.get('ckks_max_fit_error'), '.4g')} | "
                f"{fmt(r.get('ckks_approx_error_test_points'), '.4g')} | "
                f"{fmt(r.get('ckks_chain_index_start'))} -> {fmt(r.get('ckks_chain_index_end'))} | "
                f"{fmt(r.get('ckks_levels_consumed'))} |")
        lines += ["", "CKKS-specific: **FHE max abs error** is decrypted output vs the same computation in plaintext "
                  f"(PASS requires <= {meta['ckks_tolerance']:g} in every measured run and the expected levels); "
                  "**max_fit_error** is the committed fitting-grid error of the Chebyshev coefficient set and "
                  "**approx. error** the plaintext polynomial vs the true activation on the case's test points "
                  "(approximation error, kept separate from FHE error); **levels** = start - end chain index.", ""]
    lines += ["Shared: operation counts are `evaluator.<method>` calls in the generated `fhe()` "
              "(add_p = add_plain, sub_p = sub_plain, mul_p = multiply_plain, relin = relinearize, "
              "mod_sw = mod_switch_to_next); (depth, xdepth) are the CHEHAB Quantifier's (depth counts operations "
              "on the longest path, xdepth counts multiplications including ciphertext-plaintext ones). Field "
              "definitions: `docs/benchmark_results.md`."]
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0],
                                     formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    parser.add_argument("--scheme", choices=["bfv", "ckks", "all"], default="bfv")
    parser.add_argument("--benchmarks", nargs="+", help="benchmark names to run (default: all of the scheme)")
    parser.add_argument("--slot-counts", nargs="+", type=int,
                        help=f"BFV slot counts (default {BFV_DEFAULT_SLOT_COUNTS})")
    parser.add_argument("--vectorize", type=int, choices=[0, 1], default=1, help="BFV only; CKKS is always scalar")
    parser.add_argument("--opt-method", type=int, choices=[0, 1], default=1,
                        help="0 = e-graph, 1 = RL (BFV vectorized path)")
    parser.add_argument("--window", type=int, default=0)
    parser.add_argument("--cse", type=int, choices=[0, 1], default=1)
    parser.add_argument("--const-folding", type=int, choices=[0, 1], default=1)
    parser.add_argument("--compile-runs", type=int, default=3)
    parser.add_argument("--warmup", type=int, default=1, help="unmeasured runs of the generated program")
    parser.add_argument("--repeats", type=int, default=10, help="measured runs of the generated program")
    parser.add_argument("--eval-repeats", type=int, default=10, help="CKKS: timed fhe() runs inside each process")
    parser.add_argument("--ckks-tolerance", type=float, default=1e-6)
    parser.add_argument("--compile-timeout", type=float, default=7200)
    parser.add_argument("--exec-timeout", type=float, default=3600)
    parser.add_argument("--seal-prefix", default=os.environ.get("CMAKE_PREFIX_PATH", str(Path.home() / ".local")))
    parser.add_argument("--output", type=Path, help="CSV path (default results/results_<scheme>.csv)")
    parser.add_argument("--markdown", type=Path, help="also write a markdown table")
    args = parser.parse_args()
    if args.repeats < 1 or args.compile_runs < 1 or args.warmup < 0:
        parser.error("--repeats and --compile-runs must be >= 1, --warmup >= 0")

    benches = []
    if args.scheme in ("bfv", "all"):
        benches += bfv_benchmarks(args)
    if args.scheme in ("ckks", "all"):
        benches += ckks_benchmarks(args)
    if args.benchmarks:
        known = {b.name for b in benches}
        unknown = set(args.benchmarks) - known
        if unknown:
            parser.error(f"unknown benchmarks for --scheme {args.scheme}: {sorted(unknown)}; known: {sorted(known)}")
        benches = [b for b in benches if b.name in args.benchmarks]

    # Same toolchain choice as before: system gcc/g++ (the root CMakeLists also prefers them).
    env = os.environ.copy()
    if Path("/usr/bin/gcc").exists():
        env["CC"], env["CXX"] = "/usr/bin/gcc", "/usr/bin/g++"
    configure = ["cmake", "-S", ".", "-B", str(BUILD), f"-DCMAKE_PREFIX_PATH={args.seal_prefix}",
                 "-DCMAKE_BUILD_TYPE=Release"]
    if any(b.scheme == "CKKS" for b in benches):
        configure.append("-DFHECO_BUILD_CKKS_BENCHMARKS=ON")
    print("configure:", " ".join(configure), flush=True)
    run(configure, REPO, env=env)
    run(["cmake", "--build", str(BUILD), "-j8"], REPO, env=env)

    output = args.output or REPO / "results" / f"results_{args.scheme}.csv"
    output.parent.mkdir(parents=True, exist_ok=True)
    records = []
    with output.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=RECORD_FIELDS, extrasaction="ignore")
        writer.writeheader()
        for bench in benches:
            for case in bench.cases:
                record = run_case(bench, case, args, env)
                records.append(record)
                writer.writerow({k: record.get(k, "") for k in RECORD_FIELDS})
                f.flush()
                print(f"{record['record_id']:<34} {record['status']:<11} "
                      f"compile={fmt(record.get('compile_time_ms'), '.2f')} ms "
                      f"exec={fmt(record.get('exec_time_ms'), '.1f')} ms", flush=True)

    meta = {
        "commit": run(["git", "rev-parse", "--short", "HEAD"], REPO).stdout.strip(),
        "dirty": bool(run(["git", "status", "--porcelain", "--untracked-files=no"], REPO).stdout.strip()),
        "platform": f"{platform.system()} {platform.machine()}",
        "compiler": run(["c++", "--version"], REPO).stdout.splitlines()[0],
        "seal": "4.1",
        "command": "python " + " ".join([Path(sys.argv[0]).name] + sys.argv[1:]),
        "compile_runs": args.compile_runs, "warmup": args.warmup, "repeats": args.repeats,
        "eval_repeats": args.eval_repeats, "ckks_tolerance": args.ckks_tolerance,
    }
    output.with_suffix(".meta.json").write_text(json.dumps(meta, indent=2) + "\n")
    if args.markdown:
        args.markdown.parent.mkdir(parents=True, exist_ok=True)
        args.markdown.write_text(markdown(records, meta))
    failed = [r["record_id"] for r in records if r["status"] in ("FAIL", "ERROR")]
    print(f"{len(records)} records written to {output}; {len(failed)} failed"
          f"{': ' + ', '.join(failed) if failed else ''}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()

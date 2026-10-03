#!/usr/bin/env python3
"""Phase-1 CKKS baseline: every assigned operator compiled through CHEHAB.

Runs Quad, BatchNorm1d, BatchNorm2d and the eight degree-7 Chebyshev
activations through the CHEHAB CKKS path (DSL -> Compiler::compile ->
Compiler::gen_he_code -> generated SEAL program -> encrypted run), each on a
fixed and a seeded dense input vector, and writes one markdown table.

    tests/ckks/phase1_baseline.py [--output FILE] [--repeats N]

FHE numerical error is decrypted output vs the same computation in plaintext
(threshold 1e-6). For Chebyshev activations the polynomial approximation
error is reported separately: the committed fitting-grid max_fit_error and,
for information, the error on the dense test points. Builds in
<repo>/build-ckks with FHECO_BUILD_CKKS_BENCHMARKS=ON; SEAL is found through
CMAKE_PREFIX_PATH (default $HOME/.local). Exits non-zero if any run fails.
"""
import argparse
import os
import platform
import re
import shutil
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
BUILD = REPO / "build-ckks"
PREFIX = os.environ.get("CMAKE_PREFIX_PATH", str(Path.home() / ".local"))
TOLERANCE = 1e-6
ACTIVATIONS = ["gelu", "silu", "sigmoid", "elu", "selu", "softplus", "mish", "hardshrink"]
DISPLAY = {"gelu": "GELU", "silu": "SiLU", "sigmoid": "Sigmoid", "elu": "ELU", "selu": "SELU",
           "softplus": "Softplus", "mish": "Mish", "hardshrink": "Hardshrink"}


def run(cmd, cwd, check=True):
    result = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if check and result.returncode != 0:
        raise RuntimeError(f"{' '.join(map(str, cmd))} failed in {cwd}:\n{result.stdout}")
    return result


def search(pattern, text, cast=float):
    match = re.search(pattern, text)
    if not match:
        raise RuntimeError(f"pattern {pattern!r} not found in:\n{text}")
    return tuple(cast(g) for g in match.groups()) if len(match.groups()) > 1 else cast(match.group(1))


def compile_and_run(bench_dir, compile_cmd, repeats):
    """CHEHAB compile phase, then build and run the generated CKKS program."""
    compile_out = run(compile_cmd, bench_dir).stdout
    he = bench_dir / "he"
    shutil.rmtree(he / "build", ignore_errors=True)
    run(["cmake", "-S", ".", "-B", "build", f"-DCMAKE_PREFIX_PATH={PREFIX}", "-DCMAKE_BUILD_TYPE=Release"], he)
    run(["cmake", "--build", "build", "-j8"], he)
    result = run(["./main", "--tol", str(TOLERANCE), "--repeats", str(repeats)], he / "build", check=False)
    out = result.stdout
    chain_start, chain_end, levels = search(r"chain_index (\d+) -> (\d+) \(levels consumed (\d+)\)", out, int)
    return {
        "passed": result.returncode == 0,
        "fhe_error": search(r"max_abs_error=([0-9.eE+-]+)", out),
        "compile_ms": search(r"Compile time : \n([0-9.eE+-]+) ms", compile_out),
        "depth": search(r"max: \((\d+), (\d+)\)", compile_out, int),
        "exec_ms": search(r"execution_time_\(ms\): ([0-9.eE+-]+)", out),
        "eval_ms": search(r"fhe_eval_median_\(ms\): ([0-9.eE+-]+)", out),
        "chain": (chain_start, chain_end),
        "levels": levels,
    }


def benchmark(name, runs, expected_levels, extra=None):
    """runs: [(label, setup, compile_cmd)], fixed first and dense last."""
    results = {}
    for label, bench_dir, setup, compile_cmd in runs:
        setup()
        results[label] = compile_and_run(bench_dir, compile_cmd(), args.repeats)
        r = results[label]
        print(f"{name:<12} {label:<6} {'PASS' if r['passed'] else 'FAIL'} fhe_error={r['fhe_error']:.3g} "
              f"chain {r['chain'][0]}->{r['chain'][1]} eval_median={r['eval_ms']:.1f} ms", flush=True)
    fixed, dense = results["fixed"], results["dense"]
    row = {
        "name": name,
        "passed": all(r["passed"] and r["levels"] == expected_levels for r in results.values()),
        "fhe_fixed": fixed["fhe_error"],
        "fhe_dense": dense["fhe_error"],
        **{k: dense[k] for k in ("compile_ms", "exec_ms", "eval_ms", "chain", "levels", "depth")},
    }
    row.update(extra or {})
    return row


def generator_info(text):
    return {
        "approx_dense": search(r"on these points: ([0-9.eE+-]+)", text),
        "max_fit_error": search(r"max_fit_error \(fitting grid, chebyshev_coeffs.json\): ([0-9.eE+-]+)", text),
        "reference_levels": search(r"reference levels: (\d+)", text, int),
        "range": search(r"degree \d+ on \[([0-9.eE+-]+), ([0-9.eE+-]+)\]", text),
    }


parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
parser.add_argument("--output", default=str(REPO / "docs" / "ckks_phase1_baseline.md"))
parser.add_argument("--repeats", type=int, default=10)
args = parser.parse_args()

run(["cmake", "-S", ".", "-B", str(BUILD), f"-DCMAKE_PREFIX_PATH={PREFIX}", "-DCMAKE_BUILD_TYPE=Release",
     "-DFHECO_BUILD_CKKS_BENCHMARKS=ON"], REPO)
run(["cmake", "--build", str(BUILD), "-j8", "--target", "quad_ckks", "batchnorm_ckks", "generate_batchnorm_ckks",
     "chebyshev_ckks", "generate_chebyshev_ckks"], REPO)

rows = []

quad = BUILD / "benchmarks" / "quad_ckks"
quad_slots = {"fixed": ("8", "0"), "dense": ("256", "1")}
rows.append(benchmark("Quad", [
    (label, quad,
     (lambda s=s, seed=seed: run(["python3", "generate_quad_ckks.py", "--slot_count", s, "--seed", seed], quad)),
     (lambda s=s: ["./quad_ckks", s, "1", "1", "1"]))
    for label, (s, seed) in quad_slots.items()], 1, {"degree": "2 (x*x)", "range": "-"}))

bn = BUILD / "benchmarks" / "batchnorm_ckks"
for name, cases in [("BatchNorm1d", ("bn1d", "bn1d_rand")), ("BatchNorm2d", ("bn2d", "bn2d_rand"))]:
    slots = {}

    def bn_setup(case, slots=slots):
        slots[case] = run(["./generate_batchnorm_ckks", case], bn).stdout.splitlines()[0]

    rows.append(benchmark(name, [
        (label, bn, (lambda c=c: bn_setup(c)), (lambda c=c: ["./batchnorm_ckks", slots[c], "1", "1", "1"]))
        for label, c in zip(("fixed", "dense"), cases)], 1, {"degree": "1 (x*A+B)", "range": "-"}))

cheb = BUILD / "benchmarks" / "chebyshev_ckks"
for activation in ACTIVATIONS:
    info = {}

    def cheb_setup(points, activation=activation, info=info):
        out = run(["./generate_chebyshev_ckks", "p", points, activation, "7"], cheb).stdout
        info[points] = (out.splitlines()[0], generator_info(out))

    def cheb_cmd(points, activation=activation, info=info):
        return ["./chebyshev_ckks", "p", info[points][0], "1", "1", "1", activation, "7"]

    cheb_setup("dense")
    expected = info["dense"][1]["reference_levels"]
    gen = info["dense"][1]
    rows.append(benchmark(DISPLAY[activation], [
        (label, cheb, (lambda p=p: cheb_setup(p)), (lambda p=p: cheb_cmd(p)))
        for label, p in (("fixed", "points9"), ("dense", "dense"))], expected,
        {"degree": "7", "range": f"[{gen['range'][0]:g}, {gen['range'][1]:g}]",
         "max_fit_error": gen["max_fit_error"], "approx_dense": gen["approx_dense"],
         "reference_levels": expected}))

commit = run(["git", "rev-parse", "--short", "HEAD"], REPO).stdout.strip()
dirty = run(["git", "status", "--porcelain", "--untracked-files=no"], REPO).stdout.strip()
compiler = run(["c++", "--version"], REPO).stdout.splitlines()[0]

lines = [
    "# Phase-1 CKKS baseline (compiled through CHEHAB)",
    "",
    f"Generated by `tests/ckks/phase1_baseline.py` at commit {commit}{' (with local changes)' if dirty else ''}.",
    f"Platform: {platform.system()} {platform.machine()}, {compiler}, Microsoft SEAL 4.1 (Release).",
    "CKKS: N = 32768, coeff modulus {60, 40 x 10, 60}, scale 2^40, tc128 (same as the direct-SEAL reference).",
    "",
    "Every row is the DSL program compiled by `Compiler::compile` and `Compiler::gen_he_code`, built and run.",
    "",
    "| Operator | Correct? | FHE max abs error (fixed / dense) | max_fit_error (fitting grid) | Approx. error, dense points | "
    "Degree | Fitted range | CHEHAB compile time (ms) | CHEHAB execution time (ms) | Eval-only median (ms) | "
    "Chain index | Levels | CHEHAB (depth, xdepth) |",
    "|---|---|---|---|---|---|---|---|---|---|---|---|---|",
]
for r in rows:
    fit = f"{r['max_fit_error']:.4g}" if "max_fit_error" in r else "n/a"
    approx = f"{r['approx_dense']:.4g}" if "approx_dense" in r else "n/a"
    lines.append(
        f"| {r['name']} | {'PASS' if r['passed'] else 'FAIL'} | {r['fhe_fixed']:.2e} / {r['fhe_dense']:.2e} | {fit} | "
        f"{approx} | {r['degree']} | {r['range']} | {r['compile_ms']:.2f} | {r['exec_ms']:.1f} | {r['eval_ms']:.1f} | "
        f"{r['chain'][0]} -> {r['chain'][1]} | {r['levels']} | ({r['depth'][0]}, {r['depth'][1]}) |")
lines += [
    "",
    "Definitions:",
    "",
    f"- **Correct?** PASS when the decrypted output is within {TOLERANCE:g} (absolute) of the same computation in "
    "plaintext on both vectors and the levels consumed match the expectation (1 for Quad and BatchNorm; for "
    "Chebyshev, the highest index k >= 1 with |c_k| > 1e-14, plus one, as in the direct-SEAL reference).",
    "- **Vectors.** Quad: 8 values (seed 0) / 256 values (seed 1) in [-2, 2]. BatchNorm: the direct-SEAL "
    "reference tensors (N=1, C=2) / seeded random N=2, C=3 tensors (L=8; H=W=4); expected output from "
    "gamma*(x-mean)/sqrt(var+eps)+beta. Chebyshev: 9 evenly spaced points / 256 seeded uniform points over the "
    "fitted range.",
    "- **FHE max abs error** is FHE numerical error only (decrypted vs the plaintext polynomial for Chebyshev). "
    "**max_fit_error** is the committed fitting-grid error of the coefficient set "
    "(`benchmarks/dl_operators_ckks/chebyshev_coeffs.json`, 4097-point grid, not a global bound); "
    "**approx. error, dense points** is the plaintext polynomial vs the true activation on the 256 points.",
    "- **CHEHAB compile time** is CHEHAB's convention: from `create_ckks_func` through `gen_he_code`. "
    "**CHEHAB execution time** is CHEHAB's convention: parse inputs + encode + encrypt + evaluate, single run. "
    f"**Eval-only median** is `fhe()` alone, 1 warmup + {args.repeats} runs. Timings are from the dense run.",
    "- **Chain index** is that of the fresh input and of the output; **levels** = start - end.",
    "- **(depth, xdepth)** is CHEHAB's Quantifier: depth counts every operation on the longest path (excluding "
    "relin/rescale/mod_switch/match_scale); xdepth counts multiplications, including ciphertext-plaintext ones.",
]
table = "\n".join(lines) + "\n"
Path(args.output).write_text(table)
print()
print(table)
sys.exit(0 if all(r["passed"] for r in rows) else 1)

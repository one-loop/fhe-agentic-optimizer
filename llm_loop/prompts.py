"""Prompts of the LLM loop: system prompt, domain knowledge, task prompts and
compiler feedback.

Feedback follows the proposal (section 3): every attempt is answered with the
compiler's verdict and the three metrics, compilation time (C), execution
latency (L) and multiplicative depth (D), plus operation counts and the largest
estimated cost; illegal code gets the compiler's message verbatim.

Domain knowledge has three levels (--hints), so the effect of structured
grounding can be measured:
  none        CKKS rules and costs only;
  identities  plus algebraic rewrite identities;
  full        plus worked before/after examples.
The worked examples are deliberately NOT benchmark tasks (no task's answer is
given away); they illustrate general techniques.
"""
import re
from typing import List, Optional

SYSTEM_PROMPT = """You optimize Fully Homomorphic Encryption (FHE) kernels written in the C++ DSL of the CHEHAB compiler, compiled for the CKKS scheme (Microsoft SEAL backend). You work in a closed loop with the compiler: you propose a kernel, CHEHAB compiles it, the generated program runs on encrypted inputs, and you receive the compiler's verdict and measured metrics.

Goal, in order of priority: (1) lower execution latency L of the encrypted computation, (2) lower multiplicative depth D, (3) lower compilation time C, while computing exactly the same function.

# The kernel
C++ code that defines `void fhe()` (no parameters), plus any helper functions or structs it needs. It is compiled after `using namespace std; using namespace fheco;`.
- `Ciphertext x("x");` declares an encrypted input, `Plaintext p("p");` an unencrypted input, by label. Use exactly the task's input labels; an input you do not need may be left out.
- `y.set_output("y");` marks an output. Produce exactly the task's outputs with the same labels.
- Operations: `+`, `-`, `*` with at least one Ciphertext operand; unary `-c`; `square(c)`; rotations `c << k` (slot i takes slot i+k) and `c >> k`, cyclic over slots() slots; `+=`, `-=`, `*=`. `slots()` returns the slot count.
- Plain C++ (loops, ints, vectors, lambdas, helper functions, recursion) runs at compile time and only builds the computation graph.
- Not allowed: preprocessor lines, I/O, compiler internals (`Compiler::`, `util::`, `operators::`).

# CKKS rules (violations are rejected by the compiler)
1. No numeric constants in arithmetic: `c * 2`, `c + 1`, `Plaintext(3)` are rejected. Real constants come only from plaintext inputs. Write 2*c as `c + c`, and k*c by repeated doubling and addition.
2. No plaintext-plaintext operations: `p1 + p2` or `p1 * p2` between two Plaintexts are rejected at code generation. Every operation needs a Ciphertext operand. To use (p1 - p2) * c, write c * p1 - c * p2.
3. Levels: every multiplication (ciphertext x ciphertext or ciphertext x plaintext) consumes one level, and 10 levels are available. Multiplicative depth must stay <= 10.
4. Scales: two ciphertexts added or subtracted at the SAME level must come from the same pattern of multiplications; otherwise CHEHAB rejects the kernel with "CKKS scale mismatch". Operands at DIFFERENT levels are accepted: CHEHAB aligns the shallower one, which costs extra operations (a multiply by an encoded 1.0 plus a rescale, or a mod switch).
5. CHEHAB applies simplification rewrites, common-subexpression elimination, relinearization, rescaling and alignment automatically; do not emit those yourself.

# Measured costs (N = 32768; lower when fewer levels remain)
- ciphertext x ciphertext multiply (+ relinearize + rescale): ~33 ms
- rotation: ~26 ms
- ciphertext x plaintext-vector multiply (+ encode + rescale): ~8 ms; with a one-value plaintext: ~5 ms
- scale alignment inserted by CHEHAB: ~6 ms (multiply by 1.0 + rescale) or ~1 ms (mod switch)
- ciphertext addition: ~0.4 ms; adding a plaintext vector: ~2.7 ms (encoding)
Fewer ciphertext multiplications, rotations and alignments are what make a kernel faster. Lower depth alone does not: the parameters are fixed, and a lower-depth kernel that needs more multiplications is slower.

# Correctness
The kernel must compute the same function for ANY input values. It is checked on the task's test inputs and on hidden random inputs, so never rely on properties of particular test values (special coefficient values, relations between table columns, and so on). Coefficients are inputs: the kernel must be correct for any coefficient values of the terms the task lists. Masks and named constants (`one`, `half`, `alpha`) keep the meaning the task describes.

# Answer format
State the idea of your change in at most three sentences, then give the complete kernel in exactly one ```cpp code block. Always return a full kernel, never a diff."""

IDENTITIES = """
# Useful identities and techniques
- Factoring: a*b + a*c = a*(b + c) saves a multiplication (b, c ciphertexts); c*p1 + c*p2 needs two ct x pt multiplies because p1 + p2 is not allowed.
- Rotation is linear: (a << k) + (b << k) = (a + b) << k, and (a << j) << k = a << (j + k). Rotate sums, not terms.
- Rotate-and-sum over n slots takes log2(n) rotations: s = v; for (step = n/2; step >= 1; step /= 2) s += s << step.
- Chebyshev polynomials: T0 = 1, T1 = z, T_(k+1) = 2 z T_k - T_(k-1); T_(2k) = 2 T_k^2 - 1; T_(m+n) = 2 T_m T_n - T_|m-n|; T_(2k)(z) = T_k(T_2(z)).
  With u = T_2(z) = 2z^2 - 1: T_4 = 2u^2 - 1, T_6 = 4u^3 - 3u, T_3 = z(2u - 1), T_5 = z(4u^2 - 2u - 1), T_7 = z(8u^3 - 4u^2 - 4u + 1).
- Parity: an even polynomial is a polynomial in z^2 (or in u = T_2(z)); an odd one is z times a polynomial in z^2.
- Paterson-Stockmeyer / baby-step giant-step: p(z) = q0(z) + z^m q1(z) with low-degree q0, q1 evaluates a degree-d polynomial in about log2(d) depth with about sqrt(d) extra multiplications.
- Coefficient placement: multiply a coefficient into the shallowest factor of a product (c * z first, then multiply by powers), so it does not add a level at the end.
- Terms that are added should meet at different levels or share their multiplication history, to avoid scale-alignment operations.
"""

EXAMPLES = """
# Worked examples (general techniques; not from your task)

Example A: shared rotation. Before (3 rotations):
```cpp
Ciphertext out = (a << 4) + (b << 4) + (c << 8);
```
After (2 rotations): `Ciphertext out = ((a + b) + (c << 4)) << 4;`

Example B: factoring a ciphertext multiplication. Before (2 ct x ct multiplies): `y = x * w1 + x * w2;`
After (1): `y = x * (w1 + w2);`

Example C: degree-4 polynomial with only even Chebyshev terms, y = d0 + d2 T2(z) + d4 T4(z), z = x * alpha.
Before (recurrence T3 = 2 z T2 - z, T4 = 2 z T3 - T2: 3 ct x ct multiplies, 5 levels):
```cpp
Ciphertext z = x * alpha;
Ciphertext zz = z * z;
Ciphertext t2 = zz + zz - one;
Ciphertext zt2 = z * t2;
Ciphertext t3 = zt2 + zt2 - z;
Ciphertext zt3 = z * t3;
Ciphertext t4 = zt3 + zt3 - t2;
Ciphertext y = t2 * d2 + t4 * d4 + d0;
```
After (T4 = 2 u^2 - 1 with u = T2: 2 ct x ct multiplies, 4 levels):
```cpp
Ciphertext z = x * alpha;
Ciphertext zz = z * z;
Ciphertext u = zz + zz - one;
Ciphertext uu = u * u;
Ciphertext d4uu = uu * d4;
Ciphertext y = u * d2 + (d4uu + d4uu) - d4 + d0;
```
"""

HINTS = {
    "polynomial": [
        "Reduce the number of ciphertext x ciphertext multiplications.",
        "Avoid the scale-alignment operations CHEHAB inserts: make added terms share their multiplication history or meet at different levels.",
        "Change how the polynomial is evaluated (another basis, parity, a baby-step giant-step split) to need fewer multiplications.",
        "Reduce the multiplicative depth with a more balanced multiplication tree, without adding multiplications.",
    ],
    "rotation": [
        "Reduce the number of rotations (rotation is linear: rotate sums instead of separate terms).",
        "Reduce ciphertext x plaintext multiplications and plaintext encodings.",
        "Restructure the computation so partial results are shared.",
    ],
    "simple": [
        "Check whether any operation can be removed or made cheaper.",
        "Reduce ciphertext x plaintext multiplications and plaintext encodings.",
    ],
}
FAMILY_HINTS = {"chebyshev": "polynomial", "relu_composite": "polynomial", "extract": "rotation",
                "reduction": "rotation", "quad": "simple", "batchnorm": "simple", "elementwise": "simple"}


def system_prompt(hints: str) -> str:
    return SYSTEM_PROMPT + {"none": "", "identities": IDENTITIES, "full": IDENTITIES + EXAMPLES}[hints]


def task_header(task) -> str:
    return (f"# Task: {task.name}\n{task.description}\n\nCorrectness: every output within {task.tolerance:g} "
            "(absolute) of the expected values, on the test inputs and on hidden random inputs. "
            "Latency is measured on the dense test case (256 slots or the task's largest case).")


def metrics(ev, baseline, best=None, latency_source: str = "measured") -> str:
    """The feedback block for a correct kernel, in the proposal's terms (C, L, D)."""
    s, b = ev.static, baseline.static
    lat, base_lat = _latency(ev, latency_source), _latency(baseline, latency_source)
    label = "fhe() latency" if latency_source == "measured" else "estimated fhe() latency (static cost model)"
    lines = [f"Valid code (correct on every test case).",
             f"L: {label} {lat:.2f} ms (baseline {base_lat:.2f} ms, x{base_lat / lat:.2f})"
             + (f"; best so far {_latency(best, latency_source):.2f} ms" if best is not None else "") + ".",
             f"D: multiplicative depth {ev.xdepth} (baseline {baseline.xdepth}); CKKS levels used {s['levels']} of 10 "
             f"(baseline {b['levels']}).",
             f"C: compilation time {ev.compile_time_ms:.2f} ms (baseline {baseline.compile_time_ms:.2f} ms).",
             "Operations (baseline in parentheses): " + ", ".join(
                 f"{name} {s[key]} ({b[key]})" for name, key in (
                     ("ct x ct multiply", "ct_ct_mul"), ("ct x pt multiply", "ct_pt_mul"),
                     ("scale alignments", "align_mul"), ("mod switches", "mod_switch"), ("rotations", "rotate"),
                     ("additions", "add"), ("plaintext additions", "add_plain"))) + "."]
    if ev.cost_breakdown:
        total = sum(ev.cost_breakdown.values()) or 1.0
        top = max(ev.cost_breakdown.items(), key=lambda kv: kv[1])
        lines.append(f"Largest estimated cost: {top[0]}, about {100 * top[1] / total:.0f}% of the total.")
    return "\n".join(lines)


def _latency(ev, source: str) -> float:
    return ev.eval_time_ms if source == "measured" and ev.eval_time_ms is not None else ev.estimated_ms


def failure(ev, show_trace: bool = True) -> str:
    """Feedback for an illegal or wrong kernel: the compiler's verdict, verbatim."""
    msg = _clean(ev.message)
    if ev.status == "wrong_output":
        return ("Wrong result: " + msg.splitlines()[0] + ". The kernel does not compute the same function "
                "as the task. Check the algebra of your rewrite.")
    text = {"rejected": "Rejected before compiling: ", "compile_error": "Illegal code (C++ compilation failed):\n",
            "chehab_error": "Illegal code (rejected by CHEHAB):\n", "build_error": "The generated SEAL code did not build:\n",
            "run_error": "The encrypted program failed:\n", "heldout_fail": "",
            "predicted_slower": ""}.get(ev.status, "") + msg
    if "transparent" in msg:
        text += ("\nSEAL rejects a 'transparent' ciphertext: some ciphertext subtraction or sum cancels exactly "
                 "(for example (a + p) - a, which leaves only the plaintext p). CHEHAB does not catch this. Add "
                 "plaintext constants directly to a ciphertext instead of cancelling ciphertext terms.")
    if show_trace and "scale mismatch" in msg:
        text += ("\nThis is CKKS rule 4: two operands added at the same level have different scales because "
                 "they were produced by different multiplication patterns. Restructure so they meet at "
                 "different levels or share their history.")
    return text


def _clean(message: str) -> str:
    lines = [l for l in message.splitlines() if l.strip() and "File copied successfully" not in l
             and not l.startswith(("==>", "Run simplification"))]
    text = "\n".join(lines)
    text = re.sub(r"libc\+\+abi: terminating due to uncaught exception of type std::\w+: ", "", text)
    return text[:2500]


def extract_kernel(text: str) -> Optional[str]:
    """The last ```cpp block of an answer that defines fhe()."""
    blocks = re.findall(r"```(?:cpp|c\+\+|cc|C\+\+)?[ \t]*\n(.*?)```", text, flags=re.S)
    blocks = [b for b in blocks if "fhe" in b]
    return blocks[-1].strip() + "\n" if blocks else None


def idea_of(text: str) -> str:
    """The model's stated idea: the text before its code block, shortened."""
    head = text.split("```")[0].strip().replace("\n", " ")
    return head[:240] + ("..." if len(head) > 240 else "")


def code_block(source: str) -> str:
    return f"```cpp\n{source.strip()}\n```"


def history_table(entries: List[dict]) -> str:
    if not entries:
        return ""
    rows = ["| # | outcome | L (ms) | D | levels | ct x ct | ct x pt | align | rot | idea |",
            "|---|---|---:|---:|---:|---:|---:|---:|---:|---|"]
    for e in entries:
        s = e.get("static") or {}
        latency = "-" if e.get("latency") is None else f"{e['latency']:.1f}"
        rows.append(f"| {e['id']} | {e['outcome']} | {latency} | "
                    f"{e.get('xdepth', '-')} | {s.get('levels', '-')} | {s.get('ct_ct_mul', '-')} | "
                    f"{s.get('ct_pt_mul', '-')} | {s.get('align_mul', '-')} | {s.get('rotate', '-')} | "
                    f"{e.get('idea', '')[:90].replace('|', '/')} |")
    return "\n".join(rows)

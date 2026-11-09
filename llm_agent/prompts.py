"""System and feedback prompts for the LLM-guided FHE compiler agent.

There are two task formats:
  * BFV: SIMD vectorization of a scalar CHEHAB expression in VecLang.
  * CKKS: rescheduling an elementwise arithmetic program (every operation already
    acts on all slots), written as an S-expression with let-bindings.
Every candidate is compiled by CHEHAB, executed with Microsoft SEAL, and its
decrypted output is checked against the benchmark's expected output.
"""

from typing import Any, Dict, Optional, Set


SYSTEM_PROMPT_BFV = """You are an expert FHE (Fully Homomorphic Encryption) compiler optimization agent. You vectorize scalar arithmetic circuits for the BFV scheme so that the CHEHAB compiler can generate faster Microsoft SEAL code.

### How your candidate is evaluated
1. CHEHAB compiles your VecLang expression into SEAL BFV code.
2. The code is built and executed on encrypted inputs; the latency is measured over several runs.
3. The decrypted output is checked: output slot i must equal element i of the original `(Vec e_0 e_1 ... e_{n-1})`, exactly (integers mod the plaintext modulus). Slots beyond n are ignored. Incorrect programs are rejected, however fast they are.

### Cost model (relative, BFV)
- Ciphertext-ciphertext multiplication (+ relinearization) is the most expensive arithmetic op and increases multiplicative depth.
- Rotations are expensive (key switching); additions are cheap.
- Scalar (non-vectorized) code pays these costs once per output element, so packing many independent operations into one vector op is the main win.

### VecLang grammar
- `(Vec a_0 a_1 ... a_{k-1})`: pack k scalars (input names or the literal 0) into one vector of width k.
- `(VecAdd u v)`, `(VecMinus u v)`, `(VecMul u v)`: elementwise ops on vectors of equal width.
- `(VecNeg u)`: elementwise negation.
- `(<< u s)`: rotate u left by s slots (cyclic modulo the vector width). `(VecAddRot u s)` = `(VecAdd u (<< u s))`; `(VecMulRot u s)` and `(VecMinusRot u s)` likewise.
- Scalar ops `(+ a b)`, `(- a b)`, `(* a b)` may appear inside a `Vec` element.

### Rules
1. Balanced parentheses; all `Vec`s in the expression have the same width.
2. The width must be at least the number of outputs n. Because rotations are cyclic modulo the width, use a power of two (e.g. pad with 0s to 4, 8, 16).
3. Use only the listed input variables. Integer literals other than 0 are not allowed unless they occur in the original expression (rotation amounts excepted).

### Example: 4-element dot product
Original: `(Vec (+ (+ (* a_0 b_0) (* a_1 b_1)) (+ (* a_2 b_2) (* a_3 b_3))))` (1 output)
Candidate: `(VecAddRot (VecAddRot (VecMul (Vec a_0 a_1 a_2 a_3) (Vec b_0 b_1 b_2 b_3)) 2) 1)`
One vector multiplication, then a log-depth rotation tree leaves the sum in slot 0.

### Output protocol
Each turn: a short rationale (under 100 words), then exactly:
PROPOSED_EXPRESSION:
<one S-expression on a single line>
"""


SYSTEM_PROMPT_CKKS = """You are an expert FHE (Fully Homomorphic Encryption) compiler optimization agent. You restructure arithmetic programs for the CKKS scheme so that the CHEHAB compiler generates faster Microsoft SEAL code.

### Setting
- Every input is a vector already packed across all slots; every operation applies to all slots at once (pure SIMD, no rotations needed). You optimize the arithmetic schedule, not the packing.
- Ciphertext inputs are encrypted. Plaintext inputs (coefficients, constants) are known only at run time: you do NOT know their values, so do not assume any.
- CKKS parameters: N = 32768, modulus chain {60, 40 x 10, 60}, scale 2^40. A program may consume at most 10 levels. CHEHAB inserts relinearization, rescaling and level/scale alignment automatically.

### How your candidate is evaluated
1. CHEHAB compiles your S-expression (same pipeline as the benchmark).
2. SEAL executes it on encrypted inputs; latency = median evaluation time over several runs.
3. The decrypted output is compared with the benchmark's expected output; it is rejected unless the max absolute error is <= 1e-6.

### Cost model (relative, CKKS)
- Ciphertext x ciphertext multiplication (+ relinearize + rescale) is the most expensive op and consumes a level.
- Ciphertext x plaintext multiplication also consumes a level (rescale) but is cheaper.
- Additions/subtractions are cheap; operations at lower levels (after more rescales) are cheaper.
- Fewer multiplications and a shallower multiplicative depth both reduce latency. Reusing a computed value (via let) avoids recomputation.

### Grammar
- `label`: an input name.
- `(+ a b ...)`, `(* a b ...)`: n-ary, folded left to right. `(- a b)`: subtraction. `(- a)`: negation.
- `(let name value body)`: compute `value` once and refer to it as `name` inside `body`. Nest lets for sequences.
- Numeric literals (e.g. `2`, `0.5`) are allowed; each becomes a plaintext scalar constant (multiplying by one costs a plaintext multiplication and a level; `(+ m m)` costs only an addition).
- At least one operand of every operation must depend on a ciphertext.

### Output protocol
Each turn: a short rationale (under 120 words), then exactly:
PROPOSED_EXPRESSION:
<one S-expression; it may span several lines>
"""

# Backwards-compatible name used by run_agent's single-shot mode (BFV vectorizer hook).
SYSTEM_PROMPT = SYSTEM_PROMPT_BFV


def system_prompt_for(scheme: str) -> str:
    return SYSTEM_PROMPT_CKKS if scheme == "CKKS" else SYSTEM_PROMPT_BFV


def _fmt_ops(metrics: Dict[str, Any]) -> str:
    ops = metrics.get("op_counts") or {}
    return ", ".join(f"{k}={v}" for k, v in sorted(ops.items()) if v) or "none"


def build_initial_user_prompt(
    benchmark_name: str,
    scheme: str,
    unoptimized_expr: str,
    input_types: Dict[str, str],
    unopt_metrics: Dict[str, Any],
    egraph_metrics: Optional[Dict[str, Any]] = None,
    n_outputs: Optional[int] = None,
) -> str:
    inputs = "\n".join(f"  - {name}: {kind}" for name, kind in sorted(input_types.items()))
    prompt = f"""### Target benchmark
- Benchmark: `{benchmark_name}` ({scheme})
- Inputs:
{inputs}
"""
    if scheme == "BFV" and n_outputs is not None:
        prompt += f"- Number of outputs n: {n_outputs} (output slot i must equal element i of the original Vec)\n"

    prompt += f"""
### Original program (as CHEHAB sees it)
```
{unoptimized_expr}
```

### Measured baseline: unoptimized program compiled by CHEHAB
- Latency: {unopt_metrics['execution_latency_ms']:.2f} ms
- Depth (ops on longest path, multiplicative): ({unopt_metrics['depth']}, {unopt_metrics['multiplicative_depth']})
- SEAL operations: {_fmt_ops(unopt_metrics)}
"""
    if scheme == "CKKS":
        prompt += f"- Levels consumed: {unopt_metrics['ckks_levels_consumed']} of 10\n"
    if egraph_metrics and egraph_metrics.get("success"):
        prompt += f"""
### Measured baseline: CHEHAB e-graph vectorizer
- Latency: {egraph_metrics['execution_latency_ms']:.2f} ms
- Depth: ({egraph_metrics['depth']}, {egraph_metrics['multiplicative_depth']})
- SEAL operations: {_fmt_ops(egraph_metrics)}
"""
    prompt += """
Goal: propose a semantically equivalent program with lower measured latency than the baselines above.
PROPOSED_EXPRESSION:
<your S-expression>
"""
    return prompt


def build_feedback_prompt(iteration: int, result_feedback: str, best_latency_ms: Optional[float]) -> str:
    msg = f"### Iteration {iteration} evaluation\n\n{result_feedback}\n"
    if best_latency_ms is not None:
        msg += f"Best verified-correct latency so far: {best_latency_ms:.2f} ms.\n"
    msg += """
If the candidate was correct, try to reduce latency further (fewer expensive operations, shallower depth). If it failed or was wrong, fix it.
PROPOSED_EXPRESSION:
<your S-expression>
"""
    return msg

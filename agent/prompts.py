"""Prompts for the optimization agent (proposal section 3.2): a system prompt with
CHEHAB's DSL and CKKS rules, the first user turn with the task and the baseline,
and one feedback turn per evaluated candidate."""
import re
from typing import Optional

SYSTEM_PROMPT = """You optimize Fully Homomorphic Encryption (FHE) programs written in the CHEHAB compiler's C++ DSL, compiled for the CKKS scheme (Microsoft SEAL backend). You work in a loop: you propose a kernel, CHEHAB compiles it, the generated program runs on encrypted inputs, and you receive its correctness and performance. Goal: the lowest fhe() evaluation time while computing the same function.

# The kernel
A kernel is C++ code that defines `void fhe()` with no parameters, plus any helper functions or classes it needs. It is compiled after `using namespace std; using namespace fheco;`, so `vector`, `to_string` and the DSL types are available without prefixes.
- `Ciphertext x("x");` declares an encrypted input named "x"; `Plaintext p("p");` declares an unencrypted input. Use exactly the input labels of the task; an input you do not need may be left out.
- `y.set_output("y");` marks an output. Produce exactly the task's outputs, with the same labels.
- Operators: `+`, `-`, `*` between two Ciphertexts or a Ciphertext and a Plaintext; unary `-c`; `square(c)` (counted as a ciphertext-ciphertext multiplication); rotations `c << k` (slot i takes slot i+k) and `c >> k`, cyclic over slots() slots; `+=`, `-=`, `*=`. `slots()` returns the number of slots of the function.
- Plain C++ (loops, ints, vectors, lambdas, recursion) runs at compile time and only builds the computation graph.
- Not allowed: preprocessor lines, I/O, compiler internals (`Compiler::`, `util::`, `operators::`).

# CKKS rules (violations fail at compile time or give wrong results)
1. No numeric constants in arithmetic: `c * 2`, `c + 1`, `Plaintext(3)` fail ("CKKS functions do not support constants"). Real constants come only from the plaintext inputs. Write 2*c as `c + c`.
2. No plaintext-plaintext arithmetic: every operation must have a ciphertext operand.
3. Levels: every multiplication (ciphertext-ciphertext or ciphertext-plaintext) consumes one level of the operand chain, and 10 levels are available. The multiplicative depth of the longest path must stay <= 10.
4. Scales: two ciphertexts added or subtracted at the SAME level must have been produced by the same pattern of multiplications, otherwise CHEHAB reports "CKKS scale mismatch ... aligning them would need an extra level". Operands at different levels are always fine: CHEHAB aligns the shallower one for free. Structure sums so that terms of different multiplication history meet at different levels.
5. CHEHAB applies its own simplification rewrites, common-subexpression elimination, relinearization, rescaling and level/scale alignment automatically. Do not try to emit those yourself.

# Costs (measured, N = 32768, one ciphertext of 16384 slots; lower when fewer levels remain)
- ciphertext x ciphertext multiply (+ relinearize + rescale): ~30 ms at the first level
- rotation: ~20 ms
- ciphertext x plaintext-vector multiply (+ rescale): ~7 ms; with a scalar plaintext (one value): ~5 ms
- plaintext-vector add: ~2.5 ms (encoding); ciphertext add: ~0.5 ms
- level/scale alignment inserted by CHEHAB: a multiply_plain + rescale (~5 ms) or a mod switch (~1 ms)
Fewer ciphertext multiplications and rotations, and work at deeper levels, are what make a kernel faster. Reducing depth also helps, because each later operation is cheaper.

# Answer format
Briefly state the idea of your change (a few sentences), then give the complete kernel in exactly one ```cpp code block. Always return a full kernel, never a diff. If the last attempt failed, fix the reported problem first."""


def first_turn(task, baseline_source: str, baseline_summary: str, iterations: int) -> str:
    return f"""# Task: {task.name}
{task.description}

Correctness: every output must be within {task.tolerance:g} (absolute) of the expected values on all test cases ({", ".join(c.name for c in task.cases)}). Timing is measured on the dense case.

# Baseline kernel
```cpp
{baseline_source.strip()}
```

# Baseline result
{baseline_summary}

# How a kernel is judged
One timing is too noisy to rank kernels: the same circuit can measure several percent faster or slower from run to run. So a correct kernel becomes the new best only if
1. its circuit differs from the baseline's, the current best's and every earlier attempt's (circuit = CHEHAB's depth and complete operation counts), and
2. it is then confirmed faster than the best when both are timed in alternating batches (at least 3% faster, winning most batches).
Rewrites that leave the circuit unchanged (renaming, reordering, regrouping that CHEHAB normalizes away) cannot win. Aim for fewer ciphertext multiplications (including square), rotations, plaintext multiplications, rescales and scale alignments, or fewer levels.

You have {iterations} attempts. Propose a faster kernel that computes the same outputs."""


def circuit_line(ev) -> str:
    ops = ev.ops
    return (f"levels {ev.levels}, depth ({ev.depth}, {ev.mult_depth}), ct-ct mul {ops.get('ct_ct_mul')}, "
            f"ct-pt mul {ops.get('ct_pt_mul')}, rotations {ops.get('rotate')}, rescale {ops.get('rescale')}, "
            f"match_scale {ops.get('match_scale')}, mod_switch {ops.get('mod_switch')}")


def verdict_note(verdict: str, earlier, remeasure, best_iteration: int) -> str:
    best = "the baseline" if best_iteration == 0 else f"attempt {best_iteration}"
    if verdict == "same_as_baseline":
        return ("Verdict: the circuit is identical to the baseline's (same depth and operation counts), so it "
                "cannot be faster; any timing difference is noise. Change the structure of the computation.")
    if verdict == "same_as_best":
        return (f"Verdict: the circuit is identical to the current best ({best}); any timing difference is noise. "
                "Change the structure of the computation.")
    if verdict == "duplicate":
        return (f"Verdict: the circuit is identical to attempt {earlier}'s, which was already evaluated. Try a "
                "structurally different idea.")
    if verdict == "slower":
        return f"Verdict: a new circuit, but not faster than the best ({best}) in its single timing."
    if verdict in ("new_best", "not_confirmed"):
        r = remeasure
        against = "the baseline" if r["against"] == 0 else f"attempt {r['against']}"
        how = (f"Timed against {against} in {r['batches']} alternating batches: "
               f"{r['candidate_ms']:.2f} ms vs {r['reference_ms']:.2f} ms (x{r['speedup']:.2f}, faster in "
               f"{r['wins']}/{r['batches']} batches).")
        return (f"Verdict: new best. {how}" if verdict == "new_best"
                else f"Verdict: not confirmed faster, so the best is unchanged. {how}")
    return "Verdict: failed, so it does not count. Fix the reported problem, or go back to the best kernel."


def feedback_turn(iteration: int, iterations: int, result_summary: str, verdict: str, earlier, remeasure,
                  baseline, best, best_iteration: int) -> str:
    left = iterations - iteration
    name = "the baseline" if best_iteration == 0 else f"attempt {best_iteration}"
    return f"""# Result of attempt {iteration}
{result_summary}

{verdict_note(verdict, earlier, remeasure, best_iteration)}

Baseline: {baseline.eval_time_ms:.2f} ms; {circuit_line(baseline)}.
Best so far ({name}): {best.eval_time_ms:.2f} ms; {circuit_line(best)}.
{f"{left} attempt(s) left. Propose a kernel with a different, cheaper circuit than the best (start from the best one if this attempt was worse or failed)." if left else "That was the last attempt."}"""


def no_code_turn(iteration: int, iterations: int) -> str:
    return (f"# Result of attempt {iteration}\nNo ```cpp code block was found in your answer, so nothing was "
            f"evaluated. {iterations - iteration} attempt(s) left. Answer with exactly one ```cpp block "
            "holding the complete kernel.")


def extract_kernel(text: str) -> Optional[str]:
    """The last ```cpp (or ```c++ / ```) block of the answer."""
    blocks = re.findall(r"```(?:cpp|c\+\+|cc|C\+\+)?[ \t]*\n(.*?)```", text, flags=re.S)
    blocks = [b for b in blocks if "fhe" in b]
    return blocks[-1].strip() + "\n" if blocks else None

# Experimental protocol (Phase 1)

Frozen on 2026-11-20, before any Phase-1 run. Every number in the paper's main results comes from runs made
under this protocol. Results from earlier runs (`results/phase2_*`, `feedback_ablation`, `model_comparison`)
used the fixed chain, single correctness checks and 29 tasks; they are pilot data and are not pooled with
Phase-1 results.

Items marked **(team decision)** are proposed defaults that the team should confirm before Phase 1 starts.
Changing anything else after Phase 1 starts means a new protocol version and new runs.

## 1. What is measured

- **Latency.** Evaluation-only time of `fhe()` on encrypted inputs (dense case), from neutral re-scoring
  (`rescore.py`): all kernels of a task and its baseline in one session, 1 warmup + 10 measured processes of
  `./main --repeats 10`, interleaved round-robin. Per-kernel latency = median of the per-process medians.
- **Speedup** = baseline latency / kernel latency, both from the same session.
- **CKKS modulus chain: sized** (`--ckks-chain sized`) for the baseline *and* every kernel: one 40-bit prime
  per level the kernel consumes, plus the 60-bit first and special primes (N = 32768, scale 2^40, at most 10
  levels). With the fixed 10-level chain, saved levels earned almost nothing and extra levels made operations
  cheaper; the sized chain is what a deployment would use. The model is told that the chain is sized
  (`prompts.SIZED_SYSTEM_PROMPT`).
- **BFV** (nn_*, matmul, polynomial trees): N = 16384, CoeffModulus::BFVDefault, 20-bit plaintext modulus,
  scalar layout (every input its own ciphertext). Not affected by the chain setting.
- **Compile time** (median of 3) is reported, not optimized.

## 2. Correctness

- During the search: every kernel runs on the fixed and dense cases; the strategy's final kernel also on
  the 6 held-out cases (random inputs and coefficients; two edge seeds at the ends of the input range).
  Expected outputs come from an independent NumPy oracle.
- At re-scoring: every timed kernel passes the 8 cases, every timing process, and **20 more runs of every
  case** (fresh keys and encryption noise in each process; `--correctness-repeats 20`).
- **Primary metric: strict speedup.** A final kernel that fails any of these checks counts as x1.000 (the
  run falls back to the baseline). The non-strict speedup is reported alongside.
- Tolerances: 1e-6 absolute (CKKS), 1e-4 for the rotation/reduction tasks (extract*, table_mult,
  embedding), 1e-5 for `relu_composite` (its own baseline exceeded 1e-6 on 3 of 20 repeats), exact for BFV.

## 3. Tasks

34 tasks (`tasks.py`), in two sets fixed from the baselines' structure, not from results:

| set | tasks | use |
|---|---:|---|
| `main` | 25 | headline numbers: 11 Chebyshev activations, composite ReLU, extract (3), table_mult, embedding, nn_linear, nn_conv2d, nn_avgpool2d, matmul 2x2 and 3x3, 3 Coyote polynomial trees |
| `trivial` | 9 | reported separately: 6 elementwise ops, quad, batchnorm1d/2d. The baseline is already a minimal circuit, so these can only show timing noise |

New in Phase 0: `matmul_2x2`, `matmul_3x3` (the proposal's matmul; scalar `fhe()` of benchmarks/matrix_mul)
and `coyote_tree_100_50_5_1`, `coyote_tree_50_50_10_2`, `coyote_tree_100_100_5_1` (benchmarks/polynomials_coyote).
Inputs come from `gen_io.py` (seeded). Still missing from the proposal: conv1d and degree-15 polynomials
(need new coefficient sets).

## 4. Search settings

- Budget: **8 model calls per run**; every invocation counts (repairs, duplicates, failures).
- Hints `none`, feedback `execution` (measured latency), selection margin 2%, static pre-screen 15% (archive
  only; cost model chain-aware, calibrated 2026-11-20), `--final-rounds 0` (re-scoring does the timing).
- Strategies: `independent` (no feedback), `linear` (ComPilot dialogue), `archive` (REx/FunSearch-style).
- Task and strategy order: rotated per run (Latin square), so drift over a session favours nothing.
- **Runs: 5 per task, strategy and model (team decision).** From the pilot's paired SD of log speedup
  (0.0855), 125 pairs per comparison (25 tasks x 5 runs) detect a ~2% effect at 80% power.
- **Models (team decision):** Haiku 5.5 on everything; Sonnet 5.5 on everything if usage allows; Opus 5.5
  on a subset declared before its runs start.

## 5. Analysis (declared before the runs)

- Per task: mean strict speedup over runs. Across tasks: geometric mean over the `main` set.
- Three primary comparisons, runs paired by (task, run): archive vs independent, linear vs independent,
  archive vs linear. For each: geometric-mean ratio, 95% bootstrap interval (runs resampled within task,
  10,000 draws), sign-flip permutation test; **Holm correction** across the three.
- Secondary (exploratory, labelled as such): best-so-far curves, failure and duplicate counts, tokens and
  time per run, per-family results, the `trivial` set.

## 6. Baselines

- **CHEHAB scalar** (the task's baseline kernel through the same driver): the reference of every speedup.
- **CHEHAB e-graph vectorizer and RL** for the BFV tasks that exist as CHEHAB benchmarks (matrix_mul,
  polynomials_coyote; nn_* if the vectorizer accepts them): `run_benchmarks.py --vectorize 1 --opt-method 0|1`, timed in the same session
  as the re-scoring. Their programs use a packed input layout, so only `fhe()` latency is compared.
  The vectorizer runs `cargo run`; without a Rust toolchain, put `llm_loop/tools/cargo-shim` first on PATH
  (it runs the prebuilt `egraphs/target/release/egraphs-vectorization`, and refuses if it is older than its
  sources). **Status (2026-11-20): runs, correctness not established.** matrix_mul 2 and 3 compile (27 ms) and
  run in 49 ms and 117 ms (3 runs), against 208 ms and 720 ms for the scalar baselines (3 vs 27 ciphertext
  multiplies for 3x3). But `run_benchmarks.py` reports FAIL: the adapted input file CHEHAB writes
  (`fhe_io_example_adapted.txt`) lists all-zero expected outputs, so the check cannot pass. This must be fixed
  and the outputs verified before the e-graph numbers can be used.
- **Coyote**: not installed on this machine. **Not yet available.**

## 7. Machine

- One experiment at a time on one machine; nothing else heavy running.
- **AC power, lid open**: `loop.py` (at every run start) and `rescore.py` (at start, and recorded after every
  task's timing) refuse to run on battery or with the lid closed unless `--allow-unstable-power` is given;
  the state is recorded in `result.json` and `rescore.meta.json`. The scripts run under `caffeinate -i`.
- Recorded: git commit and dirty flag, compiler, SEAL version, proposer and model versions.

## 8. How to run

```bash
python3 -m llm_loop.tests.test_phase0          # offline checks, no model calls
llm_loop/experiments/phase1.sh                 # E1 with Haiku 5.5: 3 strategies x 25 tasks x 5 runs x 8 calls
MODEL=claude-sonnet-5-5 llm_loop/experiments/phase1.sh
```

Interrupted (usage limit, unplugged, lid closed): rerun the same command; completed runs are kept and an
interrupted run restarts from scratch.

## 9. Changes from the pilot runs

| | pilot | Phase 1 |
|---|---|---|
| CKKS chain | fixed 10 levels | sized to the kernel (baseline too) |
| system prompt | "lower depth alone does not help" | chain is sized, saved levels pay; BFV: packing with masks can make rotations useful |
| correctness at re-scoring | 8 cases once | 8 cases + 20 repeats of each; strict speedup |
| relu_composite tolerance | 1e-6 | 1e-5 |
| tasks | 29 | 34, `main` / `trivial` sets |
| cost model (pre-screen) | linear in primes left | also scales with chain length (sized chain only) |
| CKKS error messages | scale mismatch without operands | operands' levels, scales and expressions (from sized-chain-and-history) |
| power / lid | not checked | enforced and recorded |

## 10. Feedback study (protocol v1.1, fixed 2026-11-21 before its runs)

The paper's focus moves to the feedback system. This study runs before Phase-1 E1 and uses everything above
except where stated here.

**Question.** What does compiler feedback to the model contribute, which part of it matters, and does steering
the model toward approaches it has not tried help?

**Arms** (all: Haiku 5.5, 8 calls, hints none, sized chain, margin 2%, same selection of the final kernel):

| arm | what the model sees after each attempt |
|---|---|
| `independent` | nothing (8 stateless calls with the same first prompt) |
| `linear-minimal` | valid / correct or not, with the compiler's message when not; no latency, operation counts or ranking; pre-screen off (its verdict would reveal the cost model's estimate) |
| `linear` | the full feedback: L, D, C, operation counts, best so far |
| `linear-explore` | `linear` + a ledger of the approaches tried (stated technique, result, operation counts), kernels with the same operation counts as an earlier one marked as repeats, and the instruction to use an approach not in the ledger (the last call may refine the best kernel instead). Answers start with a `Technique:` line |
| `archive` | as in section 4 |
| `archive-explore` | `archive` + the same ledger and instruction in every non-repair prompt |

`independent`, `linear-minimal` and `linear` share the first prompt and system prompt, so their call-1 results
are a sanity check. The explore arms add the `Technique:` paragraph to the system prompt.

**Tasks:** the `feedback` set (12 tasks of `main`, chosen for time; `tasks.py` gives the reasons): GELU, SiLU,
Sigmoid, ELU, Softplus, ReLU and HardShrink (degree 7), extract_sparse, nn_conv2d, matmul 2x2 and 3x3,
coyote_tree_100_50_5_1. **Runs:** 3 per task and arm (36 pairs per comparison; from the pilot's SD this
detects about a 4% effect at 80% power, about 5% after the Holm correction). Order: all arms' run 1, then run 2, then run 3, with task and arm order
rotated per run.

**Re-scoring (v1.1 change to section 2):** the 20 correctness repeats run for the baseline and for every kernel
faster than the baseline. A kernel at or below x1.000 is not repeated: strict scoring could only raise an
unreliable slow kernel to x1.000, so skipping is conservative. BFV kernels (exact integer arithmetic; a failure
would mean noise-budget exhaustion) get 2 repeats. Everything else in sections 1-2 is unchanged.

**Primary comparisons** (paired by task and run, geometric-mean ratio of per-task mean strict speedups, 95%
bootstrap interval, sign-flip test (Monte Carlo, 100,000 draws), Holm correction over the five):

1. `linear` vs `independent`: does feedback help?
2. `linear` vs `linear-minimal`: does the performance content of the feedback help beyond correctness?
3. `linear-explore` vs `linear`: does steering help the dialogue?
4. `archive` vs `linear`: does search help on top of feedback?
5. `archive-explore` vs `archive`: does steering help the search?

**Secondary (exploratory):** every arm vs `independent`; diversity per run (distinct kernels, duplicates,
distinct operation-count structures, structure repeats, distinct techniques); best-so-far curves; failures;
tokens and time.

**Run:** `llm_loop/experiments/feedback_study.sh` (results in `llm_loop/results/feedback_study_haiku-5-5/`,
report `feedback_ablation.md`). Offline checks: `python3 -m llm_loop.tests.test_feedback_study`.

# LLM-Guided FHE Compiler Optimization Agent

An agentic closed-feedback loop optimization framework for Fully Homomorphic Encryption (FHE) code using the **CHEHAB** compiler and **Microsoft SEAL 4.1**.

This module implements **Phase 2 (The LLM Component)** of the NYU Abu Dhabi Capstone project:
> **"Optimizing Fully Homomorphic Encryption Code Using LLM-Guided Compiler Feedback"**  
> *Umair Hafeez, Saad Sifar | Advised by Riyadh Baghdadi & Eliseo Ferrante*

---

## 1. Architecture & Closed-Feedback Loop

```
+-----------------------------------------------------------------------------------+
|                            Inputs for Initialization                             |
|  - Domain System Prompt (FHE constraints: mult depth, rotation costs, packing)    |
|  - Original Unoptimized Scalar FHE IR Expression (expression.txt)                 |
|  - Baseline Measurements (Unoptimized vs. CHEHAB EGraph Heuristic)                |
+-----------------------------------------+-----------------------------------------+
                                          |
                                          v
                    +------------------------------------+
                    |             LLM Agent              |
                    | (Gemini 2.5 Flash, Claude 3.5      |
                    |  Haiku, GPT-4o-mini, or Mock)      |
                    +-----------------+------------------+
                                      |
                     Proposes S-Expr  |  Compiler Feedback:
                     Candidate        |  {Latency L, Depth D, Noise Budget,
                                      |   Speedup, or Syntax Error}
                                      v
                    +------------------------------------+
                    |        System & Evaluation         |
                    |         (CHEHAB Compiler)          |
                    |                                    |
                    |  1. Fast Grammar & AST Check       |
                    |  2. CHEHAB Vector IR Compiler      |
                    |  3. Microsoft SEAL 4.1 C++ Build   |
                    |  4. Encrypted Runtime Benchmark    |
                    +------------------------------------+
```

### How candidates are evaluated (all numbers are measured and verified)
Every baseline and candidate is compiled by CHEHAB, the generated SEAL code is built and executed, and the decrypted output is checked. Nothing is estimated and no default values are substituted: a failed step is reported as failed.

- **BFV**: candidates are VecLang vectorizations compiled through CHEHAB's vectorized path (`./<bench> 1 <slots> 2 ...`). CHEHAB writes placeholder zeros as expected outputs in that mode, so the harness checks the decrypted slots itself. The reference is the scalar expression CHEHAB emits (`expression.txt`), evaluated mod t on the generator's inputs and first cross-checked against the generator's expected outputs. Latency is the median end-to-end time of the generated `main` (1 warmup + 5 runs). Integer literals that are not in the original expression are rejected, so a candidate cannot hard-code outputs.
- **CKKS**: candidates are elementwise programs (`+ - *`, unary `-`, `(let name value body)`, numeric literals), compiled by `benchmarks/ckks_candidate` through the same CHEHAB CKKS pipeline as the benchmarks. The generated `main` compares the decrypted output with the generator's expected output (max abs error <= 1e-6). Latency is the median over 3 processes of the eval-only median. The starting program given to the LLM is the benchmark's exact computation (Chebyshev recurrence, composite sign polynomial, ...). It is compiled through the candidate path and must reproduce the benchmark's operation counts before any LLM turn runs.
- **E-graph baseline**: CHEHAB's e-graph vectorizer, BFV only (it calls `Func::require_bfv`). The compiler uses the prebuilt `egraphs/target/release/egraphs-vectorization` when it exists (or `CHEHAB_EGRAPH_BIN`), falling back to `cargo run`.
- **Isolation**: CHEHAB's benchmark binaries share `build/benchmarks/{expression,inputs,vectorized_code,candidate_vectorized_code}.txt`. A lock on `build/benchmarks/.llm_harness.lock` refuses concurrent harness runs, and stale generated files are deleted before every compile.
- **Feedback to the LLM**: measured latency, (depth, multiplicative depth), SEAL operation counts, noise budget or CKKS error/levels, or the exact compiler/build/runtime error. Wrong results are reported as wrong (slot indices for BFV, max error for CKKS) without revealing expected values.

---

## 2. Directory Structure

```
llm_agent/
├── __init__.py               # Package metadata
├── grammar.py                # VecLang AST parser, validator, and slot-length extractor
├── cost_model.py             # Static analytical FHE cost and depth evaluator
├── compiler_interface.py    # CHEHAB compiler & SEAL execution bridge
├── prompts.py                # FHE domain system prompt, few-shots, and feedback formatting
├── client.py                 # Multi-provider client (Gemini, OpenAI, Anthropic, Mock)
├── agent.py                  # Closed-loop iterative controller & best-candidate tracker
├── run_agent.py              # Command-line driver for running experiments
└── README.md                 # Complete documentation and usage guide
```

---

## 3. Quick Start & Usage

### A. Run with Deterministic Mock / Simulation (No API Key Required)
You can run the full closed loop immediately without any API keys or network requests:

```bash
# Optimize 4-element Dot Product (3 iterations)
python -m llm_agent.run_agent --benchmark dot_product --slot-count 4 --iterations 3 --provider mock

# Optimize nn_linear layer (2 iterations)
python -m llm_agent.run_agent --benchmark nn_linear --slot-count 4 --iterations 2 --provider mock

# Optimize nn_avgpool2d layer (2 iterations)
python -m llm_agent.run_agent --benchmark nn_avgpool2d --slot-count 8 --iterations 2 --provider mock
```

### B. Run with Frontier LLM Models

#### 1. Google Gemini (Gemini 2.5 Flash / Gemini 1.5 Flash)
```bash
export GEMINI_API_KEY="your-gemini-api-key"
python -m llm_agent.run_agent \
  --benchmark dot_product \
  --slot-count 4 \
  --iterations 10 \
  --provider gemini \
  --model gemini-2.5-flash
```

#### 2. OpenAI (GPT-4o-mini / GPT-4o)
```bash
export OPENAI_API_KEY="your-openai-api-key"
python -m llm_agent.run_agent \
  --benchmark nn_linear \
  --slot-count 4 \
  --iterations 10 \
  --provider openai \
  --model gpt-4o-mini
```

#### 3. Anthropic (Claude Haiku 5.5 / 4.5)
```bash
# Key can be placed in .env or exported
python -m llm_agent.run_agent \
  --benchmark dot_product \
  --slot-count 4 \
  --iterations 5 \
  --provider anthropic \
  --model claude-haiku-5-5
```

---

## 4. Benchmark Results & Verification

Each suite run writes to `results/<model>_<timestamp>/`: one `llm_opt_<benchmark>_*.json` per benchmark (prompts, replies, candidates, compiler diagnostics, measurements), plus `suite_summary.csv` and `suite_summary.md`.

Result files written before 2026-11-08 directly in `results/` (`llm_suite_*`, `llm_opt_*`) came from the previous harness. That harness estimated CKKS latencies, did not check outputs, and substituted default baseline values, so its numbers are not valid. The verified runs are in `results/claude-haiku-5-5_20261108_verified/` and `results/claude-sonnet-5-5_20261108_verified/`.

---

## 6. Running the Full Benchmark Suite (Automated Batch Runner)

To run multi-turn optimization experiments across all benchmarks and automatically aggregate results into unified CSV and Markdown tables, use `run_llm_benchmarks.py`:

```bash
cd /Volumes/SaveHere/capstone/CHEHAB

# 1. Run on Deep Learning operators (nn_linear, nn_conv2d, nn_avgpool2d) with 8 attempts
python run_llm_benchmarks.py --suite nn --iterations 8 --model claude-haiku-5-5

# 2. Run on Core kernels (DL operators + dot_product + matrix_mul) with 8 attempts
python run_llm_benchmarks.py --suite core --iterations 8 --model claude-haiku-5-5

# 3. Run on ALL 13 benchmarks across the suite
python run_llm_benchmarks.py --suite all --iterations 8 --model claude-haiku-5-5

# 4. Run on a custom selection of benchmarks
python run_llm_benchmarks.py --benchmarks nn_linear,nn_avgpool2d,dot_product --iterations 8 --model claude-haiku-5-5
```

Outputs are generated in `results/<model>_<timestamp>/` (or `results/<--output-tag>/`):
- `suite_summary.csv`: one row per benchmark, with measured latencies, speedups (only when both sides were measured and verified), verified-candidate counts and status.
- `suite_summary.md`: the same table, plus the measurement method and the best verified candidate per benchmark.

Run suites one at a time; a second concurrent run is refused by the build-directory lock. Measurement options: `--warmup`, `--repeats` (BFV), `--ckks-processes`, `--ckks-eval-repeats`, `--ckks-tolerance`.




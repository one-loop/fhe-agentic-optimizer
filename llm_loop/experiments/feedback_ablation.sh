#!/usr/bin/env bash
# Feedback ablation: does compiler feedback to the model help?
#
#   independent  no feedback: 8 stateless calls with linear's first prompt (task, baseline, baseline result);
#                the loop still verifies and times every kernel and keeps the fastest correct one
#   linear       ComPilot-style dialogue: the same first prompt, then every attempt's feedback
#   archive      the loop used for the reported results (feedback, parent selection, repair turns)
#
# Same protocol as the Phase-2 pilot: --hints none, measured feedback, 8 model calls per run (every
# invocation counts), 3 runs per task and strategy, then neutral re-scoring of every run's final kernel
# in ONE session (llm_loop/rescore.py) and the comparison (experiments/compare_feedback.py).
#
# Order: task order rotates across runs and strategy order follows a Latin square, so drift over the
# session does not favour one task or strategy:
#   run 1: GELU, Sigmoid, ELU   S1 S2 S3
#   run 2: Sigmoid, ELU, GELU   S2 S3 S1
#   run 3: ELU, GELU, Sigmoid   S3 S1 S2
#
#   llm_loop/experiments/feedback_ablation.sh                      # 3 strategies x 3 tasks x 3 runs x 8 = 216 calls
#   STRATEGIES="independent linear" llm_loop/experiments/feedback_ablation.sh     # 144 calls
#   MODEL=claude-sonnet-5-5 OUT=llm_loop/runs/feedback-ablation-sonnet \
#     RESULTS=llm_loop/results/feedback_ablation_sonnet llm_loop/experiments/feedback_ablation.sh
#   PROPOSER=replay:llm_loop/tests/replay OUT=<scratch> RESULTS=<scratch> TASKS="chebyshev_gelu_d7" \
#     RUNS="1" llm_loop/experiments/feedback_ablation.sh           # offline check, no model calls
#
# Interrupted? Rerun the same command: completed runs are kept, an interrupted run restarts from scratch.
#
# Phase-1 protocol (llm_loop/PROTOCOL.md) uses this script through experiments/phase1.sh: CKKS_CHAIN=sized,
# TASKS=set:main, 5 runs. TASKS accepts task names, family:<f> and set:<s>. The defaults reproduce the
# completed ablation (fixed chain, three Chebyshev tasks).
set -euo pipefail

repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$repo"
PROPOSER="${PROPOSER:-claude-code}"
MODEL="${MODEL:-claude-haiku-5-5}"
STRATEGIES="${STRATEGIES:-independent linear archive}"
TASKS="${TASKS:-chebyshev_gelu_d7 chebyshev_sigmoid_d7 chebyshev_elu_d7}"
RUNS="${RUNS:-1 2 3}"
ITERATIONS="${ITERATIONS:-8}"
OUT="${OUT:-llm_loop/runs/feedback-ablation}"
RESULTS="${RESULTS:-llm_loop/results/feedback_ablation}"
WORK="${WORK:-build/llm_loop/feedback-ablation}"
CKKS_CHAIN="${CKKS_CHAIN:-fixed}"
CORRECTNESS_REPEATS="${CORRECTNESS_REPEATS:-20}"
# Keep the machine awake (timings select and score kernels); the scripts themselves refuse battery / closed lid.
KEEP_AWAKE=()
command -v caffeinate >/dev/null && KEEP_AWAKE=(caffeinate -i)

if [[ "$PROPOSER" == claude-code ]]; then
  unset ANTHROPIC_API_KEY ANTHROPIC_AUTH_TOKEN  # subscription only: headless mode would bill a key
  command -v claude >/dev/null || { echo "claude (Claude Code CLI) is not on PATH" >&2; exit 1; }
fi

# shellcheck disable=SC2086
read -r -a task_list <<< "$(python3 -c 'import sys; from llm_loop.tasks import select; print(" ".join(select(sys.argv[1:])))' $TASKS)"
read -r -a strategy_list <<< "$STRATEGIES"
rotate() {  # rotate <k> <items...>: the items shifted left by k
  local k=$1; shift
  local items=("$@") n=$# out=() i
  for ((i = 0; i < n; i++)); do out+=("${items[$(((i + k) % n))]}"); done
  echo "${out[@]}"
}

for run in $RUNS; do
  tasks=$(rotate $((run - 1)) "${task_list[@]}")
  for strategy in $(rotate $((run - 1)) "${strategy_list[@]}"); do
    # shellcheck disable=SC2086
    "${KEEP_AWAKE[@]}" python3 -m llm_loop.loop --tasks $tasks --strategy "$strategy" --hints none \
      --feedback execution --iterations "$ITERATIONS" --runs "$run" --final-rounds 0 --model "$MODEL" \
      --proposer "$PROPOSER" --ckks-chain "$CKKS_CHAIN" --out "$OUT/$strategy" --work "$WORK"
    for task in $tasks; do
      if [[ ! -f "$OUT/$strategy/$task/run$run/result.json" ]]; then
        echo "run $run of $strategy/$task did not complete (usage limit?); rerun this script to resume" >&2
        exit 1
      fi
    done
  done
done

"${KEEP_AWAKE[@]}" python3 -m llm_loop.rescore --runs "$OUT" --out "$RESULTS" --work "$WORK-rescore" \
  --ckks-chain "$CKKS_CHAIN" --correctness-repeats "$CORRECTNESS_REPEATS"
python3 -m llm_loop.experiments.compare_feedback --runs "$OUT" --rescore "$RESULTS" --out "$RESULTS"

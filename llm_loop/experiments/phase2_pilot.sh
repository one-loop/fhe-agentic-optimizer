#!/usr/bin/env bash
# Phase-2 pilot (approved protocol): archive vs linear search, both with
# --hints none, on GELU d7, Sigmoid d7 and ELU d7; 8 model calls per run (every
# invocation counts), 3 independent runs per task and strategy, then neutral
# re-scoring of every run's final kernel (llm_loop/rescore.py).
#
# Order: task order rotates across runs and strategy order alternates, so drift
# over the session does not favour one task or strategy:
#   run 1: GELU, Sigmoid, ELU   archive then linear
#   run 2: Sigmoid, ELU, GELU   linear then archive
#   run 3: ELU, GELU, Sigmoid   archive then linear
#
#   llm_loop/experiments/phase2_pilot.sh                       # the experiment (144 model calls)
#   PROPOSER=replay:llm_loop/tests/replay OUT=... RESULTS=... llm_loop/experiments/phase2_pilot.sh   # offline check
#
# Interrupted? Rerun the same command: completed runs are kept, an interrupted
# run restarts from scratch (its calls are made again).
set -euo pipefail

repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$repo"
PROPOSER="${PROPOSER:-claude-code}"
OUT="${OUT:-llm_loop/runs/phase2-pilot}"
RESULTS="${RESULTS:-llm_loop/results/phase2_pilot}"
WORK="${WORK:-build/llm_loop/phase2}"

if [[ "$PROPOSER" == claude-code ]]; then
  unset ANTHROPIC_API_KEY ANTHROPIC_AUTH_TOKEN  # subscription only: headless mode would bill a key
fi

tasks_for_run() {
  case "$1" in
    1) echo "chebyshev_gelu_d7 chebyshev_sigmoid_d7 chebyshev_elu_d7" ;;
    2) echo "chebyshev_sigmoid_d7 chebyshev_elu_d7 chebyshev_gelu_d7" ;;
    3) echo "chebyshev_elu_d7 chebyshev_gelu_d7 chebyshev_sigmoid_d7" ;;
  esac
}
strategies_for_run() {
  case "$1" in
    2) echo "linear archive" ;;
    *) echo "archive linear" ;;
  esac
}

for run in 1 2 3; do
  for strategy in $(strategies_for_run "$run"); do
    # shellcheck disable=SC2046
    python3 -m llm_loop.loop --tasks $(tasks_for_run "$run") --strategy "$strategy" --hints none \
      --feedback execution --iterations 8 --runs "$run" --final-rounds 0 \
      --proposer "$PROPOSER" --out "$OUT/$strategy" --work "$WORK"
    for task in $(tasks_for_run "$run"); do
      if [[ ! -f "$OUT/$strategy/$task/run$run/result.json" ]]; then
        echo "run $run of $strategy/$task did not complete (usage limit?); rerun this script to resume" >&2
        exit 1
      fi
    done
  done
done

python3 -m llm_loop.rescore --runs "$OUT" --out "$RESULTS" --work "$WORK-rescore"

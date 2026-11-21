#!/usr/bin/env bash
# Feedback-study PILOT (overnight): the six arms of experiments/feedback_study.sh on 6 tasks, in two stages, each
# ending with neutral re-scoring and the report, so a usable report exists after stage 1:
#   stage 1: run 1 of every arm and task  (~6 h search + ~1.5 h re-scoring)  -> $RESULTS/run1/
#   stage 2: adds run 2 and re-scores all (~6 h search + ~2.5 h re-scoring)  -> $RESULTS/runs1-2/
# Pilot data: exploratory, NOT pooled with the full study (PROTOCOL.md section 10).
#
#   llm_loop/experiments/feedback_pilot.sh
#
# Interrupted? Rerun the same command: completed runs are kept (a finished stage is re-scored again, ~1.5 h).
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MODEL="${MODEL:-claude-haiku-5-5}"
tag="${MODEL#claude-}"
export MODEL
export CKKS_CHAIN="${CKKS_CHAIN:-sized}"
# One fast task per family with headroom: three Chebyshev activations, one rotation task, two BFV tasks.
export TASKS="${TASKS:-chebyshev_gelu_d7 chebyshev_sigmoid_d7 chebyshev_elu_d7 extract_sparse matmul_2x2 coyote_tree_100_50_5_1}"
export STRATEGIES="${STRATEGIES:-independent linear-minimal linear linear-explore archive archive-explore}"
export CORRECTNESS_REPEATS="${CORRECTNESS_REPEATS:-20}"
export OUT="${OUT:-llm_loop/runs/feedback-pilot-$tag}"
base_results="${RESULTS:-llm_loop/results/feedback_pilot_$tag}"
export WORK="${WORK:-build/llm_loop/feedback-pilot-$tag}"
STAGES="${STAGES:-1;1 2}"

IFS=';' read -r -a stages <<< "$STAGES"
for runs in "${stages[@]}"; do
  export RUNS="$runs"
  export RESULTS="$base_results/runs${runs// /-}"
  [[ "$runs" == 1 ]] && RESULTS="$base_results/run1"
  if [[ -f "$RESULTS/feedback_ablation.md" ]]; then
    echo "== stage RUNS=\"$runs\" already reported in $RESULTS; skipping"; continue
  fi
  echo "== $(date '+%a %H:%M') stage RUNS=\"$runs\" starting"
  "$here/feedback_ablation.sh"
  echo "== $(date '+%a %H:%M') stage RUNS=\"$runs\" done: $RESULTS/feedback_ablation.md"
done

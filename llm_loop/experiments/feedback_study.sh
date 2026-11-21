#!/usr/bin/env bash
# Feedback study (llm_loop/PROTOCOL.md section 10): six arms at equal budget (8 calls), hints none, measured
# feedback, sized CKKS chain, the `feedback` task set (12 tasks), 3 runs, then neutral re-scoring (protocol v1.1
# repeats) and the comparison with the declared primary comparisons (Holm-corrected).
#
#   independent      no feedback (best-of-8 with verification)
#   linear-minimal   dialogue, feedback = correct / incorrect + compiler message only
#   linear           dialogue, full feedback (latency, depth, operation counts, ranking)
#   linear-explore   linear + ledger of approaches tried + "use a new approach"
#   archive          REx/FunSearch-style search with full feedback
#   archive-explore  archive + the same steering
#
#   llm_loop/experiments/feedback_study.sh                       # Haiku 5.5: 6 x 12 x 3 runs x 8 = 1,728 calls
#   RUNS="1 2" llm_loop/experiments/feedback_study.sh            # a shorter session; RUNS="3" later adds run 3
#
# Runs are interleaved (all arms' run 1, then run 2, ...), with task and arm order rotated per run, so a session
# cut short still leaves complete, balanced runs. Interrupted? Rerun the same command: completed runs are kept.
# Re-scoring and the report start only after every run is complete.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MODEL="${MODEL:-claude-haiku-5-5}"
tag="${MODEL#claude-}"
export MODEL
export CKKS_CHAIN="${CKKS_CHAIN:-sized}"
export TASKS="${TASKS:-set:feedback}"
export RUNS="${RUNS:-1 2 3}"
export STRATEGIES="${STRATEGIES:-independent linear-minimal linear linear-explore archive archive-explore}"
export CORRECTNESS_REPEATS="${CORRECTNESS_REPEATS:-20}"
export OUT="${OUT:-llm_loop/runs/feedback-study-$tag}"
export RESULTS="${RESULTS:-llm_loop/results/feedback_study_$tag}"
export WORK="${WORK:-build/llm_loop/feedback-study-$tag}"
exec "$here/feedback_ablation.sh"

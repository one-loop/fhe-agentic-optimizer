#!/usr/bin/env bash
# Phase-1 experiment E1 under the frozen protocol (llm_loop/PROTOCOL.md): independent / linear / archive at
# equal budget (8 calls), hints none, measured feedback, sized CKKS chain, the `main` task set, 5 runs, then
# neutral re-scoring with 20 repeated correctness checks per case and the strict speedup.
#
#   llm_loop/experiments/phase1.sh                                   # Haiku 5.5
#   MODEL=claude-sonnet-5-5 llm_loop/experiments/phase1.sh
#   MODEL=claude-opus-5-5 TASKS="<the pre-declared Opus subset>" RUNS="1 2 3" llm_loop/experiments/phase1.sh
#
# Interrupted (usage limit, unplugged, lid closed)? Rerun the same command: completed runs are kept.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MODEL="${MODEL:-claude-haiku-5-5}"
tag="${MODEL#claude-}"
export MODEL
export CKKS_CHAIN="${CKKS_CHAIN:-sized}"
export TASKS="${TASKS:-set:main}"
export RUNS="${RUNS:-1 2 3 4 5}"
export STRATEGIES="${STRATEGIES:-independent linear archive}"
export CORRECTNESS_REPEATS="${CORRECTNESS_REPEATS:-20}"
export OUT="${OUT:-llm_loop/runs/phase1-$tag}"
export RESULTS="${RESULTS:-llm_loop/results/phase1_$tag}"
export WORK="${WORK:-build/llm_loop/phase1-$tag}"
exec "$here/feedback_ablation.sh"

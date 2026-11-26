#!/usr/bin/env bash
# Compiler ladder (experiments/compiler_ladder.py): every task's baseline unoptimized and through each CHEHAB
# ruleset, and the LLM loop's best kernel, timed in one session per task.
#
#   llm_loop/experiments/compiler_ladder.sh 2>&1 | tee -a llm_loop/runs/compiler-ladder.log
#
# Waits until the Mac is on AC power with the lid open, then runs under caffeinate. Rerunning the same command
# resumes: tasks already in partial_rows.json are skipped. The tasks with an LLM kernel go first.
set -euo pipefail
cd "$(dirname "$0")/../.."

OUT="${OUT:-llm_loop/results/compiler_ladder}"
PRIOR="${PRIOR:-llm_loop/results/feedback_pilot_haiku-5-5/runs1-2}"
RUNS="${RUNS:-llm_loop/runs/feedback-pilot-haiku-5-5}"
FIRST="chebyshev_gelu_d7 chebyshev_sigmoid_d7 chebyshev_elu_d7 extract_sparse matmul_2x2 coyote_tree_100_50_5_1"
REST=$(python3 -c "
from llm_loop.tasks import TASKS
first = '$FIRST'.split()
print(' '.join(t for t in sorted(TASKS) if t not in first))")

until python3 -c "
from llm_loop.machine import power_source, lid_closed
import sys; sys.exit(0 if power_source() == 'AC Power' and lid_closed() is False else 1)"; do
  echo "$(date '+%a %H:%M') waiting for AC power with the lid open..."
  sleep 60
done

echo "== $(date '+%a %H:%M') compiler ladder: 34 tasks -> $OUT"
caffeinate -i python3 -m llm_loop.experiments.compiler_ladder --out "$OUT" --prior $PRIOR --runs $RUNS \
  --tasks $FIRST $REST
echo "== $(date '+%a %H:%M') done: $OUT/ladder.md"

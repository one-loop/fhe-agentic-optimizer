"""Which correct candidate counts as an improvement.

One fhe() median is too noisy to rank kernels: identical circuits differ by up to
~40% on sub-millisecond tasks. So a candidate replaces the best only if
  1. its circuit differs from the baseline's, the best's and every earlier attempt's
     (the circuit fingerprint: CHEHAB's depth and complete operation counts), and
  2. it is faster than the best in an interleaved remeasurement: both programs are
     timed in alternating batches, and the candidate must win most batches and be
     at least MARGIN faster in the median of the per-batch time ratios.
"""
import math
import statistics
from typing import Dict, Optional, Sequence, Tuple

MARGIN = 0.03  # timing noise of one batch is about 2 to 3%
BATCHES = 5  # interleaved batches when deciding a new best
FINAL_BATCHES = 7  # interleaved batches for the final baseline-versus-best speedup
MIN_WIN_FRACTION = 0.8


def fingerprint(ev: dict) -> Optional[Tuple]:
    """The circuit's shape: (depth, multiplicative depth, sorted operation counts)."""
    if ev.get("depth") is None or not ev.get("ops"):
        return None
    return ev["depth"], ev["mult_depth"], tuple(sorted(ev["ops"].items()))


def classify(ev: dict, baseline_fp, best_fp, best_ms: float, seen: Dict[Tuple, int]) -> Tuple[str, Optional[int]]:
    """(verdict, earlier attempt or None) for one evaluated candidate:
    'failed', 'same_as_baseline', 'same_as_best', 'duplicate' (of the returned attempt),
    'slower' (single median not below the best's) or 'remeasure'."""
    if not ev.get("correct"):
        return "failed", None
    fp = fingerprint(ev)
    if fp == baseline_fp:
        return "same_as_baseline", None
    if fp == best_fp:
        return "same_as_best", seen.get(fp)
    if fp in seen:
        return "duplicate", seen[fp]
    if ev.get("eval_time_ms") is None or ev["eval_time_ms"] >= best_ms:
        return "slower", None
    return "remeasure", None


def confirm(candidate_ms: Sequence[float], reference_ms: Sequence[float], margin: float = MARGIN) -> dict:
    """Decide from paired batch medians (batch i of each was timed back to back)."""
    ratios = [c / r for c, r in zip(candidate_ms, reference_ms)]
    wins = sum(1 for x in ratios if x < 1)
    median_ratio = statistics.median(ratios)
    return {
        "candidate_ms": statistics.median(candidate_ms), "reference_ms": statistics.median(reference_ms),
        "ratios": [round(x, 4) for x in ratios], "wins": wins, "batches": len(ratios),
        "median_ratio": median_ratio, "speedup": 1 / median_ratio,
        "confirmed": median_ratio <= 1 - margin and wins >= math.ceil(MIN_WIN_FRACTION * len(ratios)),
    }

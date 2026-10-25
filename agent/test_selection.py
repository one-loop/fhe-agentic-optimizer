"""Tests of the candidate selection rule: agent/.venv/bin/python -m unittest agent/test_selection.py"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from selection import classify, confirm, fingerprint  # noqa: E402

BASE_OPS = {"ct_ct_mul": 5, "ct_pt_mul": 5, "relin": 5, "rotate": 0}
FAST_OPS = {"ct_ct_mul": 3, "ct_pt_mul": 4, "relin": 3, "rotate": 0}


def ev(ops, ms, correct=True, depth=19, mult_depth=7):
    return {"correct": correct, "ops": dict(ops), "depth": depth, "mult_depth": mult_depth, "eval_time_ms": ms}


class Fingerprint(unittest.TestCase):
    def test_order_of_ops_does_not_matter(self):
        self.assertEqual(fingerprint(ev(BASE_OPS, 1)), fingerprint(ev(dict(reversed(BASE_OPS.items())), 2)))

    def test_any_count_or_depth_changes_it(self):
        self.assertNotEqual(fingerprint(ev(BASE_OPS, 1)), fingerprint(ev({**BASE_OPS, "relin": 4}, 1)))
        self.assertNotEqual(fingerprint(ev(BASE_OPS, 1)), fingerprint(ev(BASE_OPS, 1, depth=18)))

    def test_failed_compile_has_none(self):
        self.assertIsNone(fingerprint({"depth": None, "ops": {}}))


class Classify(unittest.TestCase):
    def setUp(self):
        self.base = ev(BASE_OPS, 300)
        self.base_fp = fingerprint(self.base)

    def run_classify(self, cand, best=None, seen=None):
        best = best or self.base
        return classify(cand, self.base_fp, fingerprint(best), best["eval_time_ms"], seen or {})

    def test_incorrect_is_failed(self):
        self.assertEqual(self.run_classify(ev(FAST_OPS, 100, correct=False))[0], "failed")

    def test_same_circuit_as_baseline_never_improves_even_if_much_faster(self):
        self.assertEqual(self.run_classify(ev(BASE_OPS, 150))[0], "same_as_baseline")

    def test_same_circuit_as_best(self):
        best = ev(FAST_OPS, 220)
        fp = fingerprint(best)
        self.assertEqual(self.run_classify(ev(FAST_OPS, 200), best, {fp: 2}), ("same_as_best", 2))

    def test_duplicate_of_earlier_attempt(self):
        other = {"ct_ct_mul": 4, "ct_pt_mul": 5, "relin": 4, "rotate": 0}
        seen = {fingerprint(ev(other, 0)): 3}
        self.assertEqual(self.run_classify(ev(other, 100), seen=seen), ("duplicate", 3))

    def test_new_circuit_slower_than_best(self):
        self.assertEqual(self.run_classify(ev(FAST_OPS, 300))[0], "slower")

    def test_new_faster_circuit_is_remeasured(self):
        self.assertEqual(self.run_classify(ev(FAST_OPS, 299))[0], "remeasure")


class Confirm(unittest.TestCase):
    def test_clear_win(self):
        r = confirm([220, 221, 219, 222, 220], [300, 301, 299, 303, 300])
        self.assertTrue(r["confirmed"])
        self.assertEqual(r["wins"], 5)
        self.assertAlmostEqual(r["speedup"], 300 / 220, places=2)

    def test_noise_is_not_confirmed(self):
        self.assertFalse(confirm([99, 101, 100, 98, 102], [100, 100, 100, 100, 100])["confirmed"])

    def test_small_consistent_gain_below_margin_is_not_confirmed(self):
        self.assertFalse(confirm([98.5] * 5, [100] * 5)["confirmed"])

    def test_large_median_but_too_few_wins_is_not_confirmed(self):
        self.assertFalse(confirm([80, 80, 80, 110, 110], [100] * 5)["confirmed"])

    def test_four_of_five_wins_with_margin_is_confirmed(self):
        self.assertTrue(confirm([90, 90, 90, 90, 101], [100] * 5)["confirmed"])


if __name__ == "__main__":
    unittest.main()

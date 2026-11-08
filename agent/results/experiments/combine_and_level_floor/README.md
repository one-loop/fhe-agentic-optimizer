# Combining the two Chebyshev circuit families, and the fixed-parameter artifact (2026-10-25)

No model calls. Kernels are hand-written or taken from v2 runs; every number is a 7-batch alternating timing against the stated reference, all 7/7 batch wins unless noted. Scripts: `run.py`, `run_floor.py` (written for the original laptop's paths and a scratch directory; adjust before rerunning).

Kernels (each valid for every degree-7 Chebyshev task with all coefficients; tested on elu, selu, mish):

| kernel | idea | levels | ct-ct | ct-pt |
|---|---|---:|---:|---:|
| baseline | power recurrence T_k = 2zT_{k-1} - T_{k-2}, then sum c_k T_k | 8 | 6 | 8 |
| `clenshaw.cpp` | Clenshaw recurrence (coefficients added, not multiplied); from selu repeat 3 | 8 | 6 | 2 |
| `shallow.cpp` | even/odd split in s = T2(z), terms multiplied by coefficients; from selu v2 first run | 6 | 4 | 7 |
| `combo_a.cpp` | p = R + T4*Q, R and Q each by a degree-3 Clenshaw | 5 | 7 | 4 |
| `combo_b.cpp` | even/odd split in s, each half by a degree-3 Clenshaw | 6 | 6 | 3 |
| `clenshaw_burn2.cpp` | Clenshaw preceded by `x * one * one` (2 levels burned) | 10 | 6 | 4 |

## 1. As the loop measures today (fixed parameters: 10 levels for every kernel), vs the baseline

| kernel | elu | selu | mish |
|---|---:|---:|---:|
| clenshaw | 1.38 | 1.42 | 1.43 |
| shallow | 1.19 | 1.19 | 1.19 |
| combo_a | 0.87 (0/7) | 0.86 (0/7) | 0.86 (0/7) |
| combo_b | 1.17 | 1.18 | 1.18 |
| clenshaw_burn2 | 1.93 | 1.91 | 1.92 |

## 2. Fair-parameters proxy: every kernel, baseline included, first drops its unused levels (finishes at level 10), vs the equally treated baseline

| kernel | elu | selu | mish |
|---|---:|---:|---:|
| clenshaw | 1.42 | 1.42 | 1.42 |
| shallow | 1.49 | 1.51 | 1.49 |
| combo_a | 1.43 | 1.40 | 1.39 |
| **combo_b** | **1.60** | **1.58** | **1.59** |

The treated baseline itself runs at ~270 ms instead of ~360 ms: dropping its 2 unused levels alone is worth x1.34. The proxy drops levels with `x * one` (a plaintext multiply and rescale each), which is slower than a real mod switch or smaller parameters, so it slightly understates the benefit for kernels that drop more levels.

## Reading

- With fixed parameters, levels a kernel does not use are wasted, while operations lower in the chain are cheaper. Reducing depth is therefore worth almost nothing, and burning levels is rewarded (x1.42 → x1.92 for the same Clenshaw circuit). This is why the agent never kept a combined kernel: under today's measurement it is not faster.
- Under fair treatment the ranking flips: the combined kernel B is the best (about x1.59, +12% over Clenshaw), shallow beats Clenshaw, and burning gives nothing because every kernel already starts at its lowest usable level.
- So the measurement, not the agent, is what most needs fixing: dropping unused levels for every kernel (baseline included) both removes the level-burning exploit and credits real depth reductions.

## 3. After the fix: modulus chain sized to each kernel (`CkksParams::trim_to_levels_used`)

CHEHAB now generates one 40-bit prime per level the kernel consumes (plus the 60-bit first and special primes). All 26 baselines stay correct, and every generated chain matches the kernel's levels (0-level tasks get `{60, 60}`). selu, vs the new baseline (241 ms instead of 359 ms), 7/7 batch wins each:

| kernel | levels | speedup |
|---|---:|---:|
| clenshaw | 8 | 1.49 |
| clenshaw_burn2 | 10 | 1.28 (burning now costs) |
| shallow | 6 | 1.80 |
| combo_a | 5 | 1.73 |
| **combo_b** | 6 | **1.96** |

Baselines that used few of the 10 levels got much faster (table_mult 1294 → 61 ms, embedding 1279 → 139 ms, extract_sparse 220 → 14 ms), so speedups from runs made before this change are not comparable with runs after it.

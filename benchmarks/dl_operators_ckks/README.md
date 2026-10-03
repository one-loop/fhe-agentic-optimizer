# CHEHAB CKKS neural-network operators (direct-SEAL baseline)

This directory implements the three assigned operator families directly in
Microsoft SEAL CKKS. It is **not** compiled by the CHEHAB compiler (CHEHAB's
`src/fheco` is currently BFV-only); it is the regression oracle that a future
CHEHAB CKKS path must match.

1. **BatchNorm1d / BatchNorm2d**
   - Inference-time BatchNorm is pre-folded to `y = A*x + B`, where
     `A = gamma / sqrt(running_var + eps)` and `B = beta - A*running_mean`.
   - `BatchNorm1d` and `BatchNorm2d` use the same encrypted kernel; only the
     per-channel plaintext coefficient layout differs.

2. **Chebyshev smooth activations**
   - One generic Chebyshev-basis evaluator (`chebyshev()`), recurrence
     `T_k = 2 z T_{k-1} - T_{k-2}` over `z = 2(x - a)/(b - a) - 1`.
   - Coefficients are fitted offline; encrypted execution only evaluates the
     fitted polynomial. Degree d uses d + 1 levels, so degree 9 is the maximum
     with the current modulus chain.

3. **Quad**
   - Exact `x^2` activation using one ciphertext-ciphertext square,
     relinearization, and rescale.

CKKS scales are tracked exactly rather than reset to 2^40 after rescaling; see
the comment at the top of `fhe_operators.hpp` and `results/scale_fix_note.md`.

## Build and run

From the CHEHAB root (requires Microsoft SEAL 4.1 discoverable by CMake):

```bash
cmake -S . -B build-ckks-operators \
  -DFHECO_BUILD_CKKS_OPERATOR_DEMO=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-ckks-operators --target ckks_operator_demo -j8
./build-ckks-operators/benchmarks/dl_operators_ckks/ckks_operator_demo [--repeats N]
```

The module also builds on its own with `cmake -S . -B build` from this
directory. The program prints one summary table and exits non-zero if any
check fails.

## What the harness measures

| Column | Meaning |
|---|---|
| `status` | PASS if `max_fhe_err <= 1e-4` (and, for BatchNorm, the folded `A*x+B` matches the reference to 1e-12) |
| `max_fhe_err` | max abs difference between decrypted output and a plaintext reference of the same computation. BatchNorm's reference is the original `gamma*(x-mean)/sqrt(var+eps)+beta`, independent of the folding and packing helpers |
| `approx_err` | Chebyshev only: plaintext polynomial vs true activation **on the test points**. Not part of PASS/FAIL. Use `max_fit_error` in `chebyshev_coeffs.json` (fitting grid) for the approximation quality of a set |
| `depth` | ciphertext-ciphertext multiplicative depth of the evaluation structure |
| `chain(s->e)`, `levels` | chain index of the fresh input and of the output; levels consumed = start - end |
| `out_log2s` | log2 of the output ciphertext scale |
| `median_ms (min..max)` | encrypted operator only (plaintext encoding inside the operator included; encryption, decryption and key generation excluded), 1 warmup + N timed runs (default 10) |

The 1e-4 tolerance is the conservative value from the project handoff. Observed
errors are 1e-9..1e-7, so it has wide headroom but would not catch a small
systematic bias (the old forced-scale bias was 5.7e-6).

Note that CHEHAB's own benchmark harness (`run_benchmarks.py`) times
input parsing, encoding and encryption together with evaluation, and its
"multiplicative depth" (Quantifier `xdepth`) also counts ciphertext-plaintext
multiplications. Compare numbers across the two only after aligning these
definitions.

## Activation coefficients

All eight activations (GELU, SiLU, Sigmoid, ELU, SELU, Softplus, Mish,
Hardshrink) at degree 7 on [-4, 4], plus the degree-5 Sigmoid set used by the
encrypted test, are committed in `chebyshev_coeffs.json` (with fit metadata)
and `chebyshev_coeffs.hpp` (for the C++ harness). Regenerate them, byte for
byte, with:

```bash
python3 generate_chebyshev_coeffs.py --emit-all .
```

To fit a single set ad hoc:

```bash
python3 generate_chebyshev_coeffs.py --activation gelu --degree 7 --min -5 --max 5 --json
```

Use activation ranges collected from representative plaintext/calibration data.

To run another activation through the encrypted test, add its
`(activation, degree)` to `encrypted_activation_tests` in `main.cpp`; the set
must exist in `COEFF_SETS` in the generator. Plaintext reference activations
live in `activations.hpp`.

## Notes for the later optimization agent

The Chebyshev implementation is intentionally a clear recurrence-based baseline
with linear depth. It creates an optimization target for CHEHAB/LLM
transformations such as balanced power trees, Clenshaw evaluation,
Paterson-Stockmeyer evaluation, coefficient pruning, and BatchNorm fusion into
neighboring linear layers.

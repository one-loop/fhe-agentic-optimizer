# CHEHAB CKKS neural-network operators

This directory implements the three requested operator families in Microsoft SEAL CKKS:

1. **BatchNorm1d / BatchNorm2d**
   - Inference-time BatchNorm is pre-folded to `y = A*x + B`, where
     `A = gamma / sqrt(running_var + eps)` and `B = beta - A*running_mean`.
   - `BatchNorm1d` and `BatchNorm2d` use the same encrypted kernel; only the
     per-channel plaintext coefficient layout differs.

2. **Chebyshev smooth activations**
   - Generic Chebyshev-basis evaluator over an empirical input interval.
   - `generate_chebyshev_coeffs.py` supports GELU, SiLU, Sigmoid, ELU, SELU,
     Softplus, Mish, and Hardshrink.
   - Fitting happens in plaintext/offline. Encrypted execution only evaluates
     the fitted polynomial.

3. **Quad**
   - Exact `x^2` activation using one ciphertext-ciphertext square,
     relinearization, and rescale.

## Build standalone

Requires Microsoft SEAL 4.1 to be installed and discoverable by CMake.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
./build/ckks_operator_demo
```

## Put into CHEHAB

Copy this directory to:

```text
CHEHAB/benchmarks/dl_operators_ckks/
```

If CHEHAB's top-level CMake does not already include the benchmark directory,
add:

```cmake
option(FHECO_BUILD_CKKS_OPERATOR_DEMO "Build CKKS neural-network operator demo" OFF)
if(FHECO_BUILD_CKKS_OPERATOR_DEMO)
    add_subdirectory(benchmarks/dl_operators_ckks)
endif()
```

Then build with:

```bash
cmake -S . -B build-ckks-operators \
  -DFHECO_BUILD_CKKS_OPERATOR_DEMO=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-ckks-operators --target ckks_operator_demo -j2
./build-ckks-operators/benchmarks/dl_operators_ckks/ckks_operator_demo
```

## Generate activation coefficients

Examples:

```bash
python3 generate_chebyshev_coeffs.py --activation sigmoid --degree 5 --min -4 --max 4
python3 generate_chebyshev_coeffs.py --activation gelu --degree 7 --min -5 --max 5
python3 generate_chebyshev_coeffs.py --activation mish --degree 7 --min -5 --max 5 --json
```

Use activation ranges collected from representative plaintext/calibration data.

## Notes for the later optimization agent

The Chebyshev implementation is intentionally a clear recurrence-based baseline.
It creates an optimization target for CHEHAB/LLM transformations such as
balanced power trees, Clenshaw evaluation, Paterson-Stockmeyer evaluation,
coefficient pruning, and BatchNorm fusion into neighboring linear layers.

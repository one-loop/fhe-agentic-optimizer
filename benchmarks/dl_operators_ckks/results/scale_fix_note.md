# CKKS scale-management fix: before/after

Environment: Apple silicon (arm64), macOS 26.6.2, Apple clang 17.0.0,
Microsoft SEAL 4.1 (Release), CHEHAB commit 70c3b0d + module changes.
CKKS: N = 32768, coeff modulus {60, 40 x 10, 60}, scale 2^40.

## Before (commit 70c3b0d, forced `ct.scale() = 2^40` after every rescale)

Three consecutive runs of `ckks_operator_demo`:

```
Quad               latency_ms=40.401 max_error=5.723e-06
BatchNorm1d        latency_ms=13.748 max_error=3.301e-06
BatchNorm2d        latency_ms=11.145 max_error=3.243e-06
ChebyshevSigmoid   latency_ms=137.135 max_error=2.202e-06
  polynomial approximation max_error=1.058e-02
Quad               latency_ms=36.877 max_error=5.731e-06
BatchNorm1d        latency_ms=11.759 max_error=3.289e-06
BatchNorm2d        latency_ms=11.289 max_error=3.244e-06
ChebyshevSigmoid   latency_ms=129.794 max_error=2.229e-06
  polynomial approximation max_error=1.058e-02
Quad               latency_ms=35.922 max_error=5.724e-06
BatchNorm1d        latency_ms=11.224 max_error=3.299e-06
BatchNorm2d        latency_ms=10.684 max_error=3.228e-06
ChebyshevSigmoid   latency_ms=126.784 max_error=2.206e-06
  polynomial approximation max_error=1.058e-02
```

The error was nearly constant across runs because it was a deterministic bias,
not CKKS noise. The SEAL-generated 40-bit primes sit 1.4e-6 to 9.8e-6 below
2^40, and relabelling the post-rescale scale s^2/q as s scales every decoded
value by q/2^40. Example: the first rescale drops the prime
q = 2^40 * (1 - 1.43e-6); Quad's largest output is 4, and 4 * 1.43e-6 =
5.72e-6, the observed error.

## After (scale tracked exactly; see header comment in fhe_operators.hpp)

Three consecutive runs:

```
Quad               latency_ms=40.196 max_error=2.428e-08
BatchNorm1d        latency_ms=11.163 max_error=2.243e-08
BatchNorm2d        latency_ms=10.520 max_error=9.854e-09
ChebyshevSigmoid   latency_ms=139.696 max_error=1.947e-08
  polynomial approximation max_error=1.058e-02
Quad               latency_ms=37.675 max_error=4.101e-08
BatchNorm1d        latency_ms=10.804 max_error=1.634e-08
BatchNorm2d        latency_ms=10.707 max_error=2.597e-08
ChebyshevSigmoid   latency_ms=138.336 max_error=3.361e-08
  polynomial approximation max_error=1.058e-02
Quad               latency_ms=37.518 max_error=1.190e-08
BatchNorm1d        latency_ms=10.894 max_error=1.270e-08
BatchNorm2d        latency_ms=11.265 max_error=8.563e-09
ChebyshevSigmoid   latency_ms=165.295 max_error=1.263e-08
  polynomial approximation max_error=1.058e-02
```

Errors are now at the CKKS noise floor (1e-9 to 1e-7) and vary from run to run.
Level consumption is unchanged: Quad 1, BatchNorm 1, Chebyshev degree d uses
d + 1 levels (degree 9 is still the maximum for this modulus chain). The
polynomial approximation error is unaffected.

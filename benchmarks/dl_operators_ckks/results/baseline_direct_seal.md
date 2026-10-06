# Direct-SEAL CKKS baseline results

Regression oracle for the future CHEHAB CKKS path. Not CHEHAB-compiled.

Environment: Apple silicon (arm64), macOS 26.6.2, Apple clang 17.0.0,
Microsoft SEAL 4.1 (Release), branch chehab-ckks-operator-integration
(module at commit 8219eba plus this README/results commit),
numpy 2.4.6 for coefficient fitting.
CKKS: N = 32768, coeff modulus {60, 40 x 10, 60}, scale 2^40, fresh inputs at
chain index 10. Column meanings are in ../README.md.

## Default run (`ckks_operator_demo`, 1 warmup + 10 timed runs)

```
operator                 status  max_fhe_err  approx_err   depth  chain(s->e)   levels  out_log2s   median_ms (min..max)
Quad                     PASS    4.418e-08    n/a          1      10->9         1       40.000002   35.978 (35.274..37.454)
BatchNorm1d              PASS    1.124e-08    n/a          0      10->9         1       40.000000   10.412 (10.269..10.826)
BatchNorm2d              PASS    1.256e-08    n/a          0      10->9         1       40.000000   10.554 (10.442..10.641)
Chebyshev-sigmoid-d5     PASS    7.761e-09    1.058e-02    4      10->4         6       40.000000   131.623 (130.710..133.781)
depth = ciphertext-ciphertext multiplicative depth; levels = start chain index - end chain index
latency: encrypted operator only, 1 warmup + 10 timed runs
tolerance: |fhe - plaintext reference| <= 1.0e-04 (BatchNorm folding <= 1.0e-12)
```

## All eight activations (not run by default)

Same binary with every degree-7 set added to `encrypted_activation_tests`,
1 warmup + 5 timed runs. `approx_err` here is on 9 test points only; see
`max_fit_error` in `chebyshev_coeffs.json` for the fitting-grid value (e.g.
Hardshrink: 0.051 on the test points vs 0.464 on the grid). GELU, SiLU and
Softplus use one level fewer because their degree-7 coefficient is ~1e-17 and
is skipped.

```
operator              status  max_fhe_err  approx_err   depth  chain(s->e)   levels  out_log2s   median_ms (min..max)
Quad                  PASS    5.200e-08    n/a          1      10->9         1       40.000002   36.486 (35.751..37.146)
BatchNorm1d           PASS    5.029e-09    n/a          0      10->9         1       40.000000   11.026 (10.753..11.601)
BatchNorm2d           PASS    7.478e-09    n/a          0      10->9         1       40.000000   10.708 (10.560..10.757)
Chebyshev-sigmoid-d5  PASS    1.428e-08    1.058e-02    4      10->4         6       40.000000   132.572 (131.069..132.664)
Chebyshev-gelu-d7     PASS    6.739e-08    5.815e-02    6      10->3         7       40.000000   171.913 (170.257..182.192)
Chebyshev-silu-d7     PASS    5.474e-08    1.726e-02    6      10->3         7       40.000000   169.107 (168.549..170.994)
Chebyshev-sigmoid-d7  PASS    1.234e-08    2.824e-03    6      10->2         8       40.000000   168.879 (167.877..169.784)
Chebyshev-elu-d7      PASS    4.734e-08    4.275e-02    6      10->2         8       40.000000   177.378 (176.844..177.778)
Chebyshev-selu-d7     PASS    4.635e-08    9.476e-02    6      10->2         8       40.000000   178.602 (177.702..179.423)
Chebyshev-softplus-d7 PASS    2.366e-08    3.572e-03    6      10->3         7       40.000000   169.769 (169.452..186.635)
Chebyshev-mish-d7     PASS    3.528e-08    3.818e-02    6      10->2         8       40.000000   179.591 (177.593..180.362)
Chebyshev-hardshrink-d7PASS    1.987e-08    5.066e-02    6      10->2         8       40.000000   168.713 (166.979..169.300)
depth = ciphertext-ciphertext multiplicative depth; levels = start chain index - end chain index
latency: encrypted operator only, 1 warmup + 5 timed runs
tolerance: |fhe - plaintext reference| <= 1.0e-04 (BatchNorm folding <= 1.0e-12)
```

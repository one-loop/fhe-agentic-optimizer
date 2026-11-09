# LLM-Guided FHE Optimization: Benchmark Suite
- **Date**: 2026-11-08 22:04:04
- **Model**: `claude-sonnet-5-5` (anthropic), 6 iterations per benchmark
- **Host**: Darwin arm64 26.6, Microsoft SEAL 4.1.1
- **Wall-clock**: 29.0 min

## How the numbers were obtained
- Every latency is measured: the program is compiled by CHEHAB, the generated SEAL code is built and run.
- BFV latency: median end-to-end time of the generated `main` (parse + encode + encrypt + evaluate), 1 warmup + 5 measured runs.
- CKKS latency: median over 3 processes of the eval-only median (1 warmup + 5 runs of `fhe()`).
- Correctness is verified for every reported program. BFV: decrypted output slots must equal the benchmark's scalar expression evaluated mod t on the generator's inputs (cross-checked against the generator's expected outputs). CKKS: decrypted output vs the generator's expected output, max abs error <= 1e-06. Only verified candidates can be 'best'.
- CKKS has no e-graph baseline: CHEHAB's vectorizer is BFV-only.
- '—' means no valid measurement exists; no value is ever substituted.

## Results

| Benchmark | Scheme | Slots | Unoptimized (ms) | CHEHAB e-graph (ms) | Best LLM (ms) | LLM vs unopt | LLM vs e-graph | e-graph vs unopt | Verified candidates | Status |
| :--- | :--- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | :---: | :--- |
| `nn_linear` | BFV | 4 | 548.51 | 60.45 | 60.00 | 9.14x | 1.01x | 9.07x | 6/6 | OK |
| `nn_conv2d` | BFV | 8 | 9032.18 | 339.97 | 338.90 | 26.65x | 1.00x | 26.57x | 1/6 | OK |
| `nn_avgpool2d` | BFV | 8 | 310.76 | 19.53 | 18.86 | 16.48x | 1.04x | 15.92x | 6/6 | OK |
| `quad_ckks` | CKKS | 8 | 36.25 | n/a (vectorizer is BFV-only) | 36.33 | 1.00x | — | — | 6/6 | OK |
| `batchnorm1d_ckks` | CKKS | 8 | 10.09 | n/a (vectorizer is BFV-only) | 10.04 | 1.00x | — | — | 6/6 | OK |
| `batchnorm2d_ckks` | CKKS | 8 | 10.04 | n/a (vectorizer is BFV-only) | 9.80 | 1.02x | — | — | 6/6 | OK |
| `chebyshev_sigmoid_d5_ckks` | CKKS | 9 | 146.56 | n/a (vectorizer is BFV-only) | 93.46 | 1.57x | — | — | 6/6 | OK |
| `chebyshev_relu_d2_ckks` | CKKS | 9 | 59.09 | n/a (vectorizer is BFV-only) | 58.89 | 1.00x | — | — | 6/6 | OK |
| `chebyshev_gelu_d7_ckks` | CKKS | 9 | 169.57 | n/a (vectorizer is BFV-only) | 120.77 | 1.40x | — | — | 4/6 | OK |
| `relu_ckks` | CKKS | 9 | 360.91 | n/a (vectorizer is BFV-only) | 358.70 | 1.01x | — | — | 5/6 | OK |

## Best verified candidates

### `nn_linear`
- Depth (6, 1); SEAL ops: add=3, mul=1, relin=1, rotate=2; noise budget 327 bits
- Check: all 4 output slots equal the reference values
```lisp
(VecAdd (VecAddRot (VecAddRot (VecMul (Vec w_0_0 w_1_0 w_2_0 w_3_0 w_0_1 w_1_1 w_2_1 w_3_1 w_0_2 w_1_2 w_2_2 w_3_2 w_0_3 w_1_3 w_2_3 w_3_3) (Vec x_0 x_0 x_0 x_0 x_1 x_1 x_1 x_1 x_2 x_2 x_2 x_2 x_3 x_3 x_3 x_3)) 4) 8) (Vec b_0 b_1 b_2 b_3 0 0 0 0 0 0 0 0 0 0 0 0))
```

### `nn_conv2d`
- Depth (10, 1); SEAL ops: add=9, mul=9, relin=9; noise budget 327 bits
- Check: all 36 output slots equal the reference values
```lisp
(VecAdd (VecAdd (VecAdd (VecAdd (VecAdd (VecAdd (VecAdd (VecAdd (VecAdd (Vec b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (VecMul (Vec x_0_0 x_0_1 x_0_2 x_0_3 x_0_4 x_0_5 x_1_0 x_1_1 x_1_2 x_1_3 x_1_4 x_1_5 x_2_0 x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_3_0 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_4_0 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_5_0 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 k_0_0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_0_1 x_0_2 x_0_3 x_0_4 x_0_5 x_0_6 x_1_1 x_1_2 x_1_3 x_1_4 x_1_5 x_1_6 x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 k_0_1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_0_2 x_0_3 x_0_4 x_0_5 x_0_6 x_0_7 x_1_2 x_1_3 x_1_4 x_1_5 x_1_6 x_1_7 x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_2_7 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_3_7 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_4_7 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 x_5_7 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 k_0_2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_1_0 x_1_1 x_1_2 x_1_3 x_1_4 x_1_5 x_2_0 x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_3_0 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_4_0 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_5_0 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 x_6_0 x_6_1 x_6_2 x_6_3 x_6_4 x_6_5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 k_1_0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_1_1 x_1_2 x_1_3 x_1_4 x_1_5 x_1_6 x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 x_6_1 x_6_2 x_6_3 x_6_4 x_6_5 x_6_6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 k_1_1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_1_2 x_1_3 x_1_4 x_1_5 x_1_6 x_1_7 x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_2_7 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_3_7 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_4_7 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 x_5_7 x_6_2 x_6_3 x_6_4 x_6_5 x_6_6 x_6_7 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 k_1_2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_2_0 x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_3_0 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_4_0 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_5_0 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 x_6_0 x_6_1 x_6_2 x_6_3 x_6_4 x_6_5 x_7_0 x_7_1 x_7_2 x_7_3 x_7_4 x_7_5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 k_2_0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_2_1 x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_3_1 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_4_1 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_5_1 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 x_6_1 x_6_2 x_6_3 x_6_4 x_6_5 x_6_6 x_7_1 x_7_2 x_7_3 x_7_4 x_7_5 x_7_6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 k_2_1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))) (VecMul (Vec x_2_2 x_2_3 x_2_4 x_2_5 x_2_6 x_2_7 x_3_2 x_3_3 x_3_4 x_3_5 x_3_6 x_3_7 x_4_2 x_4_3 x_4_4 x_4_5 x_4_6 x_4_7 x_5_2 x_5_3 x_5_4 x_5_5 x_5_6 x_5_7 x_6_2 x_6_3 x_6_4 x_6_5 x_6_6 x_6_7 x_7_2 x_7_3 x_7_4 x_7_5 x_7_6 x_7_7 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0) (Vec k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 k_2_2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)))
```

### `nn_avgpool2d`
- Depth (3, 0); SEAL ops: add=2, rotate=1; noise budget 357 bits
- Check: all 16 output slots equal the reference values
```lisp
(VecAddRot (VecAdd (Vec x_0_0 x_0_2 x_0_4 x_0_6 x_2_0 x_2_2 x_2_4 x_2_6 x_4_0 x_4_2 x_4_4 x_4_6 x_6_0 x_6_2 x_6_4 x_6_6 x_0_1 x_0_3 x_0_5 x_0_7 x_2_1 x_2_3 x_2_5 x_2_7 x_4_1 x_4_3 x_4_5 x_4_7 x_6_1 x_6_3 x_6_5 x_6_7) (Vec x_1_0 x_1_2 x_1_4 x_1_6 x_3_0 x_3_2 x_3_4 x_3_6 x_5_0 x_5_2 x_5_4 x_5_6 x_7_0 x_7_2 x_7_4 x_7_6 x_1_1 x_1_3 x_1_5 x_1_7 x_3_1 x_3_3 x_3_5 x_3_7 x_5_1 x_5_3 x_5_5 x_5_7 x_7_1 x_7_3 x_7_5 x_7_7)) 16)
```

### `quad_ckks`
- Depth (1, 1); SEAL ops: mul=1, relin=1, rescale=1; max abs error 2.93e-08, 1 levels
- Check: max abs error 2.927e-08 vs expected outputs (tolerance 1e-06)
```lisp
(* x x)
```

### `batchnorm1d_ckks`
- Depth (2, 1); SEAL ops: add_plain=1, mul_plain=1, rescale=1; max abs error 2.27e-08, 1 levels
- Check: max abs error 2.274e-08 vs expected outputs (tolerance 1e-06)
```lisp
(+ B (* A x))
```

### `batchnorm2d_ckks`
- Depth (2, 1); SEAL ops: add_plain=1, mul_plain=1, rescale=1; max abs error 2.01e-08, 1 levels
- Check: max abs error 2.010e-08 vs expected outputs (tolerance 1e-06)
```lisp
(+ (* A x) B)
```

### `chebyshev_sigmoid_d5_ckks`
- Depth (12, 5); SEAL ops: add=3, add_plain=3, mod_switch=4, mul=3, mul_plain=2, relin=3, rescale=5, sub_plain=4; max abs error 2.95e-08, 5 levels
- Check: max abs error 2.950e-08 vs expected outputs (tolerance 1e-06)
```lisp
(let z (* x alpha) (let w (* z z) (let s (+ w w) (let u (- s 1) (let d (+ u u) (let t2 (- s one) (let b (- (+ t2 t2) 1) (let cc (+ (* d c5) c3) (let inner (- (+ (* cc b) c1) c5) (+ (* inner z) c0))))))))))
```

### `chebyshev_relu_d2_ckks`
- Depth (7, 3); SEAL ops: add=2, add_plain=1, mul=1, mul_plain=4, relin=1, rescale=5, sub_plain=1; max abs error 6.00e-08, 3 levels
- Check: max abs error 6.000e-08 vs expected outputs (tolerance 1e-06)
```lisp
(let z (* x alpha) (let zsq (* z z) (let t2 (- (+ zsq zsq) one) (+ (+ (* t2 c2) (* z c1)) c0))))
```

### `chebyshev_gelu_d7_ckks`
- Depth (10, 4); SEAL ops: add=6, add_plain=2, mul=3, mul_plain=6, relin=3, rescale=9, sub=1, sub_plain=2; max abs error 1.29e-07, 4 levels
- Check: max abs error 1.294e-07 vs expected outputs (tolerance 1e-06)
```lisp
(let z (* x alpha) (let zsq (* z z) (let u (- (+ zsq zsq) one) (let q (* u u) (let w (* u c6) (let p (+ (+ w w) c4) (let m (* q (+ p p)) (let w3 (+ (+ w w) w) (let low (- (- (+ (+ (* z c1) (* u c2)) c0) w3) c4) (+ low m))))))))))
```

### `relu_ckks`
- Depth (17, 10); SEAL ops: add=9, add_plain=1, mod_switch=9, mul=16, mul_plain=21, relin=16, rescale=37; max abs error 7.88e-08, 10 levels
- Check: max abs error 7.884e-08 vs expected outputs (tolerance 1e-06)
```lisp
(let s1p2 (* x x) (let s1p4 (* s1p2 s1p2) (let t1 (+ (+ (* x s1c1) (* (* x s1c3) s1p2)) (* (+ (* x s1c5) (* (* x s1c7) s1p2)) s1p4)) (let s2p2 (* t1 t1) (let s2p4 (* s2p2 s2p2) (let t2 (+ (+ (* t1 s2c1) (* (* t1 s2c3) s2p2)) (* (+ (* t1 s2c5) (* (* t1 s2c7) s2p2)) s2p4)) (let s3p2 (* t2 t2) (let s3p4 (* s3p2 s3p2) (let t3 (+ (+ (* t2 s3c1) (* (* t2 s3c3) s3p2)) (* (+ (* t2 s3c5) (* (* t2 s3c7) s3p2)) s3p4)) (* x (+ t3 half)))))))))))
```


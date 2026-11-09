# LLM-Guided FHE Optimization: Benchmark Suite
- **Date**: 2026-11-08 21:35:06
- **Model**: `claude-haiku-5-5` (anthropic), 6 iterations per benchmark
- **Host**: Darwin arm64 26.6, Microsoft SEAL 4.1.1
- **Wall-clock**: 17.6 min

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
| `nn_linear` | BFV | 4 | 543.20 | 60.96 | 60.03 | 9.05x | 1.02x | 8.91x | 6/6 | OK |
| `nn_conv2d` | BFV | 8 | 9012.84 | 337.85 | — | — | — | 26.68x | 0/6 | NO_VERIFIED_CANDIDATE |
| `nn_avgpool2d` | BFV | 8 | 306.64 | 18.42 | 19.35 | 15.85x | 0.95x | 16.65x | 6/6 | OK |
| `quad_ckks` | CKKS | 8 | 35.72 | n/a (vectorizer is BFV-only) | 35.77 | 1.00x | — | — | 6/6 | OK |
| `batchnorm1d_ckks` | CKKS | 8 | 9.75 | n/a (vectorizer is BFV-only) | 9.75 | 1.00x | — | — | 6/6 | OK |
| `batchnorm2d_ckks` | CKKS | 8 | 9.78 | n/a (vectorizer is BFV-only) | 9.76 | 1.00x | — | — | 6/6 | OK |
| `chebyshev_sigmoid_d5_ckks` | CKKS | 9 | 146.67 | n/a (vectorizer is BFV-only) | 171.90 | 0.85x | — | — | 1/6 | OK |
| `chebyshev_relu_d2_ckks` | CKKS | 9 | 59.01 | n/a (vectorizer is BFV-only) | 46.00 | 1.28x | — | — | 5/6 | OK |
| `chebyshev_gelu_d7_ckks` | CKKS | 9 | 169.60 | n/a (vectorizer is BFV-only) | 129.32 | 1.31x | — | — | 2/6 | OK |
| `relu_ckks` | CKKS | 9 | 359.23 | n/a (vectorizer is BFV-only) | 360.16 | 1.00x | — | — | 3/6 | OK |

## Best verified candidates

### `nn_linear`
- Depth (6, 1); SEAL ops: add=3, mul=1, relin=1, rotate=2; noise budget 328 bits
- Check: all 4 output slots equal the reference values
```lisp
(VecAdd (VecAddRot (VecAddRot (VecMul (Vec x_0 x_0 x_0 x_0 x_1 x_1 x_1 x_1 x_2 x_2 x_2 x_2 x_3 x_3 x_3 x_3) (Vec w_0_0 w_1_0 w_2_0 w_3_0 w_0_1 w_1_1 w_2_1 w_3_1 w_0_2 w_1_2 w_2_2 w_3_2 w_0_3 w_1_3 w_2_3 w_3_3)) 4) 8) (Vec b_0 b_1 b_2 b_3 0 0 0 0 0 0 0 0 0 0 0 0))
```

### `nn_conv2d`
- NO_VERIFIED_CANDIDATE

### `nn_avgpool2d`
- Depth (2, 0); SEAL ops: add=3; noise budget 360 bits
- Check: all 16 output slots equal the reference values
```lisp
(VecAdd (VecAdd (Vec x_0_0 x_0_2 x_0_4 x_0_6 x_2_0 x_2_2 x_2_4 x_2_6 x_4_0 x_4_2 x_4_4 x_4_6 x_6_0 x_6_2 x_6_4 x_6_6) (Vec x_0_1 x_0_3 x_0_5 x_0_7 x_2_1 x_2_3 x_2_5 x_2_7 x_4_1 x_4_3 x_4_5 x_4_7 x_6_1 x_6_3 x_6_5 x_6_7)) (VecAdd (Vec x_1_0 x_1_2 x_1_4 x_1_6 x_3_0 x_3_2 x_3_4 x_3_6 x_5_0 x_5_2 x_5_4 x_5_6 x_7_0 x_7_2 x_7_4 x_7_6) (Vec x_1_1 x_1_3 x_1_5 x_1_7 x_3_1 x_3_3 x_3_5 x_3_7 x_5_1 x_5_3 x_5_5 x_5_7 x_7_1 x_7_3 x_7_5 x_7_7)))
```

### `quad_ckks`
- Depth (1, 1); SEAL ops: mul=1, relin=1, rescale=1; max abs error 2.93e-08, 1 levels
- Check: max abs error 2.928e-08 vs expected outputs (tolerance 1e-06)
```lisp
(* x x)
```

### `batchnorm1d_ckks`
- Depth (2, 1); SEAL ops: add_plain=1, mul_plain=1, rescale=1; max abs error 2.28e-08, 1 levels
- Check: max abs error 2.284e-08 vs expected outputs (tolerance 1e-06)
```lisp
(+ (* x A) B)
```

### `batchnorm2d_ckks`
- Depth (2, 1); SEAL ops: add_plain=1, mul_plain=1, rescale=1; max abs error 1.19e-08, 1 levels
- Check: max abs error 1.191e-08 vs expected outputs (tolerance 1e-06)
```lisp
(+ (* x A) B)
```

### `chebyshev_sigmoid_d5_ckks`
- Depth (12, 5); SEAL ops: add=19, add_plain=1, mod_switch=6, mul=4, mul_plain=12, relin=4, rescale=16, sub=2; max abs error 4.99e-07, 5 levels
- Check: max abs error 4.990e-07 vs expected outputs (tolerance 1e-06)
```lisp
(let z (* x alpha) (let z2 (* z z) (let z3 (* z2 z) (let z4 (* z2 z2) (let z5 (* z4 z) (let a1 (* c1 z) (let a3 (* c3 z3) (let a3b (* c3 z) (let a5 (* c5 z5) (let a5b (* c5 z3) (let a5c (* c5 z) (let a3d (+ a3 a3) (let a3q (+ a3d a3d) (let a3bd (+ a3b a3b) (let a3t (+ a3bd a3b) (let a5d (+ a5 a5) (let a5q (+ a5d a5d) (let a5o (+ a5q a5q) (let a5h (+ a5o a5o) (let a5bd (+ a5b a5b) (let a5bq (+ a5bd a5bd) (let a5bo (+ a5bq a5bq) (let a5b16 (+ a5bo a5bo) (let a5b20 (+ a5b16 a5bq) (let a5cd (+ a5c a5c) (let a5cq (+ a5cd a5cd) (let a5c5 (+ a5cq a5c) (+ (+ (+ (+ c0 a1) (- a3q a3t)) (- a5h a5b20)) a5c5) )))))))))))))))))))))))))))
```

### `chebyshev_relu_d2_ckks`
- Depth (7, 3); SEAL ops: add=1, add_plain=2, mod_switch=1, mul=1, mul_plain=2, relin=1, rescale=3, sub_plain=1; max abs error 5.65e-08, 3 levels
- Check: max abs error 5.649e-08 vs expected outputs (tolerance 1e-06)
```lisp
(let u (* x alpha) (let w (* u c2) (let v (+ c1 (+ w w)) (let p (* u v) (+ (- p c2) c0)))))
```

### `chebyshev_gelu_d7_ckks`
- Depth (13, 5); SEAL ops: add=6, add_plain=1, mod_switch=2, mul=3, mul_plain=9, relin=3, rescale=12, sub=1, sub_plain=2; max abs error 5.89e-08, 5 levels
- Check: max abs error 5.888e-08 vs expected outputs (tolerance 1e-06)
```lisp
(let z (* x alpha) (let zsq (* z z) (let t2 (- (+ zsq zsq) one) (let t2sq (* t2 t2) (let t4 (- (+ t2sq t2sq) one) (let p46 (* t4 t2) (let t6 (- (+ p46 p46) t2) (+ (+ (+ (+ (* z c1) (* t2 c2)) (* t4 c4)) (* t6 c6)) c0))))))))
```

### `relu_ckks`
- Depth (17, 10); SEAL ops: add=9, add_plain=1, mod_switch=9, mul=16, mul_plain=21, relin=16, rescale=37; max abs error 1.22e-07, 10 levels
- Check: max abs error 1.222e-07 vs expected outputs (tolerance 1e-06)
```lisp
(let s1p2 (* x x) (let s1p4 (* s1p2 s1p2) (let t1 (+ (+ (* x s1c1) (* (* x s1c3) s1p2)) (* (+ (* x s1c5) (* (* x s1c7) s1p2)) s1p4)) (let s2p2 (* t1 t1) (let s2p4 (* s2p2 s2p2) (let t2 (+ (+ (* t1 s2c1) (* (* t1 s2c3) s2p2)) (* (+ (* t1 s2c5) (* (* t1 s2c7) s2p2)) s2p4)) (let s3p2 (* t2 t2) (let s3p4 (* s3p2 s3p2) (let t3 (+ (+ (* t2 s3c1) (* (* t2 s3c3) s3p2)) (* (+ (* t2 s3c5) (* (* t2 s3c7) s3p2)) s3p4)) (* x (+ t3 half)))))))))))
```


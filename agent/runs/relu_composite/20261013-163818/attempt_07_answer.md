I'm stopping here and keeping the attempt 1 kernel as the final answer. Attempts 1 through 3, 5 and 6 build the same graph (16 ciphertext multiplies, 12 plaintext multiplies, 9 level alignments) and land between 639 and 653 ms, so the differences are noise. Attempt 4 showed that restructuring the low/high sum breaks CHEHAB's scale rule, and the variants that would fix it either raise depth to 4 or add ciphertext multiplies. With 2 attempts left and no untested change that is likely to beat the noise floor, I'm not spending them on speculative rewrites.

The kernel below is the best measured version: 639.17 ms against the 672.92 ms baseline (about 5% faster), correct on every case with max error 1.75e-07.

```cpp
// Explicit odd degree-7 evaluation per stage:
//   p(t) = low(t) + t^4 * high(t),
//   low  = c1 t + (c3 t) * t^2,  high = c5 t + (c7 t) * t^2.
// Per stage: 5 ct-ct multiplies (t^2, t^4, two (c t)*t^2, high*t^4) and 4 ct-pt multiplies.
// Depth per stage is 3, so 3 stages + final x*h give 10 levels.

Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_eval(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;   // t^2
  Ciphertext v = u * u;   // t^4

  Ciphertext low = t * coeff(stage, 1) + (t * coeff(stage, 3)) * u;
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * u;

  return low + high * v;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = stage_eval(t, stage);

  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
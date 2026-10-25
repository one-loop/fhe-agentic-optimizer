Attempt 5 reproduced the attempt 1 graph (640 ms, within noise of 639 ms), so this is a plateau. Each stage needs 5 ciphertext multiplies at depth 3 to fit the 10-level budget, and the restructurings I tried either broke the scale rules or raised depth. I am submitting the attempt 1 kernel as the best measured version.

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
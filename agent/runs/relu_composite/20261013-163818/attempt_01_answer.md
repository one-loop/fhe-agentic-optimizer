**Idea:** The baseline's depth-3 degree-7 evaluation already needs 5 ciphertext-ciphertext multiplies per stage, and I could not find a depth-3 schedule with fewer. This version writes the odd polynomial explicitly as `low + high * t^4`, with `low = c1 t + (c3 t) t^2` and `high = c5 t + (c7 t) t^2`. The shared `t^2` and `t^4` are computed once per stage, and each scalar multiply is applied to `t` before the power-of-two product. The depth stays at 3 per stage, so the total is 10 levels. This is structurally close to the baseline, so I expect only a modest speedup, mainly from a cleaner operation order.

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
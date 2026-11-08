Attempt 5 is the base. The new change is to compute every power that is a pure square with `square(...)` instead of `a * a`: t², t⁴ in stages 1 and 2, and u, u² in stage 3. That is 6 of the 19 multiplications.

CHEHAB counts `square` separately, so this should produce a distinct circuit. SEAL's square is usually somewhat cheaper than a general ciphertext multiply, because it needs fewer polynomial products. I have not measured that on this backend, though, and the gain may be too small to clear the 3% bar.

Everything else is unchanged, so it should stay correct and at 9 levels.

```cpp
Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_base(const Ciphertext &t, int stage)
{
  Ciphertext t2 = square(t);
  Ciphertext t4 = square(t2);
  Ciphertext low = t * coeff(stage, 1) + (t * coeff(stage, 3)) * t2;
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * t2;
  return low + high * t4;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext X3 = x * coeff(3, 3);
  Ciphertext X5 = x * coeff(3, 5);
  Ciphertext X7 = x * coeff(3, 7);
  Plaintext half("half");

  Ciphertext t = stage_base(x, 1);
  t = stage_base(t, 2);

  Ciphertext u = square(t);                  // L7
  Ciphertext u2 = square(u);                 // L8
  Ciphertext B = t * coeff(3, 1) + half;     // L7
  Ciphertext A0 = x * B;                     // L8
  Ciphertext A3 = (X3 * t) * u;              // L8
  Ciphertext H = X5 * t + (X7 * t) * u;      // L8
  Ciphertext high = H * u2;                  // L9
  Ciphertext y = (high + A0) + A3;
  y.set_output("y");
}
```
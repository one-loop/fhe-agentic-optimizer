The previous kernel needed 11 levels, one more than the chain has. My best guess is that one of CHEHAB's rewrites in the new stage form added a level, either by factoring the shared `t` out of `hi + lo` or by pulling a scalar out of `(c7·u)·u`. I have not confirmed which.

To narrow it down and still gain, this kernel uses the cheaper form in stages 1 and 2 only and keeps the baseline form in stage 3. The cheaper form is p = c1·t + t³·(c3 + c5·u + c7·u²), which needs 4 ciphertext multiplications instead of 5. If it compiles, the kernel should have 14 ciphertext multiplications and 10 plaintext multiplications.

```cpp
Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_fast(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;
  Ciphertext c7u = u * coeff(stage, 7);
  Ciphertext c7u2 = c7u * u;
  Ciphertext c5u = u * coeff(stage, 5);
  Ciphertext inner = (c7u2 + c5u) + coeff(stage, 3);
  Ciphertext t3 = t * u;
  Ciphertext hi = t3 * inner;
  Ciphertext lo = t * coeff(stage, 1);
  return hi + lo;
}

Ciphertext stage_base(const Ciphertext &t, int stage)
{
  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext low = t * coeff(stage, 1) + (t * coeff(stage, 3)) * t2;
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * t2;
  return low + high * t4;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  t = stage_fast(t, 1);
  t = stage_fast(t, 2);
  t = stage_base(t, 3);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
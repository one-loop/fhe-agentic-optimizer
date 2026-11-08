Both failures had the same cause: a mistake in my level accounting, not a rewrite. A plaintext multiply on u (already at level 1) lands at level 2, so `(c7·u)·u` sits at level 3. That makes the 4-multiplication stage depth 4, so it cannot fit.

This attempt changes the last step instead. Rather than finishing with `y = x·(p3(t) + half)`, I fold x into the stage-3 coefficients: X_k = s3c_k·x, computed early on the fresh input. Then

y = t·(X1 + X3·u) + (X5·t + (X7·t)·u)·u² + half·x,

with t = t2 and u = t².

- **Level saving:** the X_k cost no levels on the critical path, so the circuit should need only 9 levels. That means one fewer prime for every operation in the program.
- **Extra multiplications:** this costs 2 more ciphertext multiplications than the baseline, 18 instead of 16.
- **Risk:** the two level-9 terms are products with different histories. CHEHAB may need an alignment there, which could cost back the saved level.

```cpp
Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
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
  Ciphertext X1 = x * coeff(3, 1);
  Ciphertext X3 = x * coeff(3, 3);
  Ciphertext X5 = x * coeff(3, 5);
  Ciphertext X7 = x * coeff(3, 7);
  Plaintext half("half");
  Ciphertext xh = x * half;

  Ciphertext t = stage_base(x, 1);
  t = stage_base(t, 2);

  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext A = X3 * u + X1;
  Ciphertext low = t * A;
  Ciphertext H = X5 * t + (X7 * t) * u;
  Ciphertext high = H * u2;
  Ciphertext y = (low + high) + xh;
  y.set_output("y");
}
```
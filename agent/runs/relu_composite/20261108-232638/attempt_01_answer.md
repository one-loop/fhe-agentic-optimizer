Each stage is regrouped as p(t) = c1·t + t³·(c3 + c5·u + c7·u²), with u = t².

- **Fewer multiplications:** each stage needs 4 ciphertext-ciphertext multiplications instead of 5. They are u = t·t, c7u·u, t³ = t·u, and t³ times the bracket.
- **Fewer plaintext multiplications:** each stage needs 3 instead of 4, plus one plaintext add for c3.
- **Same depth:** each stage stays at multiplicative depth 3. c1·t sits at level 1 and the t³ product at level 3. Inside the bracket, c5·u is at level 1 and c7·u² at level 2. Terms that are added together therefore never meet at the same level with different histories.
- **Totals:** the whole kernel should come to 13 ciphertext multiplications (down from 16) and 9 plaintext multiplications (down from 12), still using 10 levels.

```cpp
Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_eval(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;                       // L1
  Ciphertext c7u = u * coeff(stage, 7);       // L1
  Ciphertext c7u2 = c7u * u;                  // L2
  Ciphertext c5u = u * coeff(stage, 5);       // L1
  Ciphertext inner = (c7u2 + c5u) + coeff(stage, 3); // L2
  Ciphertext t3 = t * u;                      // L2
  Ciphertext hi = t3 * inner;                 // L3
  Ciphertext lo = t * coeff(stage, 1);        // L1
  return hi + lo;
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
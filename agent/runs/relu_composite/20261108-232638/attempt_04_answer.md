Attempt 3 showed that CHEHAB tracks scales exactly. The low part `t·(X1 + X3·u)` and the high part `H·u²` both landed at level 9 with different multiplication histories, so they could not be added.

This attempt keeps the idea of folding x into the stage-3 coefficients, which removes the final `x·h` multiply and should save a level. To avoid the conflict, the low part is now built as `X1·t + (X3·t)·u`, which finishes at level 8. The high part finishes at level 9. They meet at different levels, so CHEHAB can align them for free.

The cost is 3 more ciphertext multiplications than the baseline (19 instead of 16). In exchange, the circuit should need 9 levels instead of 10, so every operation carries one fewer prime. The levels in the comments are my own count, assuming stage 2's output t is at level 6. I have not confirmed that CHEHAB keeps this grouping rather than refactoring `t` back out of the low part.

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

  Ciphertext u = t * t;                      // L7
  Ciphertext u2 = u * u;                     // L8
  Ciphertext low = X1 * t + (X3 * t) * u;    // L8
  Ciphertext H = X5 * t + (X7 * t) * u;      // L8
  Ciphertext high = H * u2;                  // L9
  Ciphertext y = (high + low) + xh;
  y.set_output("y");
}
```
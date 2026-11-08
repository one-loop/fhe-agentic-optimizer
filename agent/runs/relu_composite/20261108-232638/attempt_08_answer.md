This is the last attempt, and I don't expect it to beat attempt 5 by the required 3%. Every restructuring I found that would cut stage-3 multiplications or the top-level plaintext multiplies by `x` fails CHEHAB's exact scale tracking. That includes merging the c3 term into the `x·B` product, building `X5·t` from `x·t`, and factoring `t` out of the high part. Spending the spare tenth level on a 4-multiplication stage 1 would add a prime to every operation, and that costs more than one multiplication saves.

This kernel is attempt 6 with only the final summation order changed to `(high + A3) + A0`. Attempt 6 measured about 0.5% faster than attempt 5, not enough to count. If CHEHAB normalizes the new order away, the circuit will be identical to attempt 6 and cannot win. If it does not, the gain should still be small.

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
  Ciphertext y = (high + A3) + A0;
  y.set_output("y");
}
```
**Diagnosis:** The error persists at level 4 because two addends in my sums sit at the same level with different histories. In `a4 = T4*c4` and the high part `T4*r`, the level collision came from my T4 being built as `T2*T2`. The baseline avoided this by giving each addend its own level, using the recurrence T_k = 2z·T_{k−1} − T_{k−2}.

**Fix:**
- Build T3 and T4 with the baseline recurrence (`T4 = 2·z·T3 − T2`). This puts T4 one level deeper, so its products land on distinct levels.
- Fold c4 into the high part: `T4·(r + c4)` equals `c4·T4 + T4·r`, using a ct+pt add. This removes the separate `a4` addend that collided with the high part.
- The low part is then `c0 + c1·z − c7·z + c2·T2 − c6·T2 + c3·T3 − c5·T3`, with each term on a distinct level.

This keeps 4 ciphertext-ciphertext multiplies (z², z·T2, z·T3, and T4·(r+c4)).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T3 = 2z*T2 - z
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  // T4 = 2z*T3 - T2
  Ciphertext zT3 = z * T3;
  Ciphertext T4 = zT3 + zT3 - T2;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // Low part: c0 + (c1-c7) z + (c2-c6) T2 + (c3-c5) T3
  Ciphertext a1 = z * c1 - z * c7;
  Ciphertext a2 = T2 * c2 - T2 * c6;
  Ciphertext a3 = T3 * c3 - T3 * c5;
  Ciphertext low = a1 + a2 + a3 + c0;

  // High part: T4 * (c4 + 2c5 z + 2c6 T2 + 2c7 T3)
  Ciphertext r1 = z * c5;
  Ciphertext r2 = T2 * c6;
  Ciphertext r3 = T3 * c7;
  Ciphertext r = (r1 + r1) + (r2 + r2) + (r3 + r3);
  Ciphertext rr = r + c4;
  Ciphertext high = T4 * rr;

  Ciphertext y = low + high;
  y.set_output("y");
}
```
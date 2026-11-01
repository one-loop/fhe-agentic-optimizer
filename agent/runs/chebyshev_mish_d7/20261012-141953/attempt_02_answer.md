**Diagnosis:** The failing `+` is at level 4, where `a3 = T3*c3 - T3*c5` meets `a4 = T4*c4`. In my last kernel, T3 was built as `zT2 + zT2 - z`, which mixes in the shallower `z` and changes its scale history relative to T4.

**Fix:** Write T3 as a single product, `T3 = z * (T2 + T2 - one)`, since 4z³ − 3z = z·(4z² − 3) = z·(2·T2 − 1). This removes the mixed-level ciphertext subtraction. It costs the same single ciphertext-ciphertext multiply as before, so the overall structure is unchanged.

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

  // T3 = z * (2*T2 - 1) = 4z^3 - 3z  (single product, no mixed-level subtraction)
  Ciphertext W = T2 + T2 - one;
  Ciphertext T3 = z * W;

  // T4 = 2*T2^2 - 1
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // Low part: c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3 + c4 T4
  Ciphertext a1 = z * c1 - z * c7;
  Ciphertext a2 = T2 * c2 - T2 * c6;
  Ciphertext a3 = T3 * c3 - T3 * c5;
  Ciphertext a4 = T4 * c4;
  Ciphertext low = a1 + a2 + a3 + a4 + c0;

  // High part: T4 * r, r = 2 c5 T1 + 2 c6 T2 + 2 c7 T3
  Ciphertext r1 = z * c5;
  Ciphertext r2 = T2 * c6;
  Ciphertext r3 = T3 * c7;
  Ciphertext r = (r1 + r1) + (r2 + r2) + (r3 + r3);
  Ciphertext high = T4 * r;

  Ciphertext y = low + high;
  y.set_output("y");
}
```
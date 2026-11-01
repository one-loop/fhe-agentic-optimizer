Idea: use the identities T5 = 2·T4·T1 − T3, T6 = 2·T4·T2 − T2 and T7 = 2·T4·T3 − T1 to factor the polynomial as

y = [c0 + c4T4 + c1T1 − c7T1 + c2T2 − c6T2 + c3T3 − c5T3] + 2·T4·(c5T1 + c6T2 + c7T3)

What this changes:
- **Ciphertext multiplications:** 4 instead of 6 (z², z·T2, T2², and T4 times the inner sum).
- **Plaintext multiplications:** 10 scalar ones instead of 8. The term c6T2 is shared between the two parts.
- **Depth:** about 5 instead of 8, so later operations run at cheaper levels.

The terms added together at the same level come from matching multiplication chains, so they should not cause a scale mismatch. I have not compiled or run this kernel yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext q = z * c5 + c6T2 + T3 * c7;
  Ciphertext p = T4 * q;

  Ciphertext low = (z * c1 - z * c7) + (T2 * c2 - c6T2) + (T3 * c3 - T3 * c5) + T4 * c4;
  Ciphertext result = low + (p + p) + c0;
  result.set_output("y");
}
```
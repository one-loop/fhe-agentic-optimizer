The quadratic in T2 now uses Horner form, so only one coefficient multiplication is needed besides alpha. With s = T2:

2·c5·T2 + 2·c7·T4 + c3 = s·(4·c7·s + 2·c5) + (c3 − 2·c7)

- e = s·c7 is the only coefficient multiplication.
- 2·c5 is added as plaintext additions.
- c3 − 2·c7 is added as plaintext additions after the ct-ct product s·g.

This drops one plaintext multiplication: 3 → 2, counting alpha. The 4 ciphertext-ciphertext multiplications and depth 6 stay the same.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;          // T2
  Ciphertext q = s + s - one;            // 2T2 - 1

  Ciphertext e = s * c7;
  Ciphertext e2 = e + e;
  Ciphertext g = e2 + e2 + c5 + c5;      // 4 c7 T2 + 2 c5
  Ciphertext inner = s * g + c3 - c7 - c7;

  Ciphertext m = q * inner + c1 - c5 - c7;
  Ciphertext result = z * m;
  result.set_output("y");
}
```
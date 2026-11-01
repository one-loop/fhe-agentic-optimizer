The polynomial y = c0 + c1·z + c2·(2z² − 1) is evaluated in Horner form: y = z·(2·c2·z + c1) + (c0 − c2), with z = x·alpha.

- **Multiplications:** this needs only two ciphertext×plaintext multiplies (by alpha and by c2) and one ciphertext×ciphertext multiply. The baseline needs three plaintext multiplies plus one ciphertext multiply.
- **Depth:** it stays at 3 levels.
- **Scales:** the two operands of the ciphertext multiply sit at different levels (2 and 1), so CHEHAB should align them without a scale mismatch error.
- **Constants:** c0 and −c2 are added as plaintexts at the end. This avoids the extra `t2 * c2` product and should also avoid the scale alignment the baseline needed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod + c0 - c2;
  result.set_output("y");
}
```
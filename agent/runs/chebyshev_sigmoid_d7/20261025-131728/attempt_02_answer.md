The earlier attempt failed because `c5*t5` and `c7*t7` met at the same level with different scale histories. This version folds the plaintext coefficients into the products before the last ct-ct multiplication. Since `T5 = 2·w·T3 − z` and `T7 = 2·T4·T3 − z`, the sum `c5·T5 + c7·T7` becomes `2·(c5·w + c7·T4)·T3 − (c5 + c7)·z`.

- **Fewer multiplications:** the two products `w·T3` and `T4·T3` merge into one ct-ct multiplication.
- **Scales:** `c5·w` is at level 3 and `c7·T4` is at level 4, so they meet at different levels and CHEHAB aligns them for free.
- **Linear terms:** the `z` terms (`c1`, `c5`, `c7`) all share the same history at level 2, so they can be added safely.
- **Cost:** this uses 5 ct-ct multiplications against the baseline's 6, and the depth is lower.

I have not run this kernel, so the scale alignment is unconfirmed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext zw = z * w;
  Ciphertext t3 = zw + zw - z;
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;
  Ciphertext q = w * c5 + t4 * c7;
  Ciphertext m = q * t3;
  Ciphertext m2 = m + m;
  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = lin + t3 * c3;
  result = result + m2;
  result = result + c0;
  result.set_output("y");
}
```
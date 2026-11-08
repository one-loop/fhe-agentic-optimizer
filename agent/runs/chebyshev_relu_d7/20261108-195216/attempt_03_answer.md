Attempt 2's rewrite saved only about 1%. CHEHAB still inserted two scale alignments (about 5 ms each): one for the level-2 term `lin` and one for the level-3 remainder.

This attempt keeps the operation counts the same and changes only how the sum is grouped:
- The level-3 remainder (c2·w − u − inner) is added straight onto the level-4 product.
- The level-2 term `c1·z` is combined with `c0` and added last.

The aim is for CHEHAB to align `lin` directly with a cheap mod switch rather than a multiply_plain + rescale.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext w = zz + zz - one;          // T2
  Ciphertext ww = w * w;                 // T2^2
  Ciphertext u = w * c6;                 // c6*T2
  Ciphertext inner = u + u + c4;         // 2 c6 T2 + c4
  Ciphertext hi = (ww + ww) * inner;     // 2 T2^2 * inner
  Ciphertext rest3 = w * c2 - u - inner;
  Ciphertext lin = z * c1 + c0;
  Ciphertext result = (hi + rest3) + lin;
  result.set_output("y");
}
```
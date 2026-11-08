Attempt 6 was faster than attempt 1 in all 5 batches, but only by about 2%. Attempt 3's grouping gained very little on its own and lost 2 of its 5 batches.

This attempt combines the two cheapest variants: `square()` for both self-products, plus attempt 3's final grouping. In that grouping, the level-3 remainder is added onto the level-4 product, and the linear term with `c0` is added last. The hope is that the two small gains add up to more than the 3% threshold.

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
  Ciphertext zz = square(z);
  Ciphertext w = zz + zz - one;          // T2
  Ciphertext ww = square(w);             // T2^2
  Ciphertext u = w * c6;                 // c6*T2
  Ciphertext inner = u + u + c4;         // 2 c6 T2 + c4
  Ciphertext hi = (ww + ww) * inner;     // 2 T2^2 * inner
  Ciphertext rest3 = w * c2 - u - inner;
  Ciphertext lin = z * c1 + c0;
  Ciphertext result = (hi + rest3) + lin;
  result.set_output("y");
}
```
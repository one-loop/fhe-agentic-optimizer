The idea is to expand T4·inner as (2w² − 1)·inner = (w² + w²)·inner − inner. This removes the "− one" plaintext add from T4; the multiplication count and depth stay the same. The −inner term then merges with the level-3 terms c2·w − u. All the low-level terms are summed first, so only one alignment up to level 4 should be needed before adding the high product.

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
  Ciphertext lin = z * c1;
  Ciphertext rest = w * c2 - u - inner + lin + c0;
  Ciphertext result = hi + rest;
  result.set_output("y");
}
```
Attempts 2 and 3 only regrouped the sums, and neither change was big enough to be confirmed. The remaining overhead is mostly the two scale alignments.

This attempt keeps attempt 2's structure, which drops the extra "− one" encode. It changes how the linear term is built: instead of z·c1, it computes (x·c1)·alpha. That gives the term a different plaintext-multiplication history at level 2, and the aim is for CHEHAB to align it more cheaply when it joins the level-3 remainder.

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
  Ciphertext xc = x * c1;
  Ciphertext lin = xc * alpha;           // c1 * z
  Ciphertext rest = w * c2 - u - inner + lin + c0;
  Ciphertext result = hi + rest;
  result.set_output("y");
}
```
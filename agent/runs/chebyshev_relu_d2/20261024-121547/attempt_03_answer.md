The remaining overhead is small, so I'm keeping the Horner form: y = (c0 − c2) + z·(c1 + 2·c2·z). Three plaintext factors (alpha, c2, alpha) multiply x², so depth 2 isn't reachable. The rewrite uses the minimum of one ct-ct multiply and two ct-pt multiplies. My last attempt resubmitted the same kernel, so this one changes the order of the constant additions. I add `c0` and subtract `c2` in a different order, in case that changes how CHEHAB schedules the plaintext encodings and alignment.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext s = w + c1;
  s = s + w;
  Ciphertext r = z * s;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```
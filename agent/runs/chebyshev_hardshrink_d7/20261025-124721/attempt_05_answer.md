The scale mismatch came from adding a plaintext-multiply result and a ciphertext-ciphertext result at the same level. I now group c5·a5 + c7·a7 as a5·(c5 + c7·v) − c7·a3. The ciphertext-ciphertext term sits at level 4, and the plaintext-multiply terms (c3·a3, c7·a3) sit at level 3, so they meet at different levels and CHEHAB aligns them for free.

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
  Ciphertext w = z * z;
  Ciphertext w2 = w + w;
  Ciphertext v = w2 + w2 - one - one;
  Ciphertext a3 = v - one;
  Ciphertext a5 = a3 * v - one;
  Ciphertext cv = v * c7 + c5;
  Ciphertext low = a3 * c3 - a3 * c7 + c1;
  Ciphertext p = a5 * cv + low;
  Ciphertext result = z * p;
  result.set_output("y");
}
```
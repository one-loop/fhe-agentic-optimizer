The output is odd in z, so y = z·P(w) with w = z². Each T_k/z is a polynomial in w, built from v = 4w − 2:

- a3 = v − 1
- a5 = a3·v − 1
- a7 = a5·v − a3

Then P = c1 + c3·a3 + c5·a5 + c7·a7 and y = z·P. This still uses 4 ct-ct multiplies, but c1 is now a plaintext add (no level, ~2.5 ms) instead of a plaintext multiply. That removes one ct-pt multiply, and the depth drops by one level.

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
  Ciphertext a7 = a5 * v - a3;
  Ciphertext p = a3 * c3 + a5 * c5 + a7 * c7 + c1;
  Ciphertext result = z * p;
  result.set_output("y");
}
```
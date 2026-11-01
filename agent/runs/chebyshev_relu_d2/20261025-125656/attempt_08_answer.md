Idea: form 2z² directly as a product of the two level-1 ciphertexts `(z+z)*z`, with no mod switch. Then apply `c2` to it, and add the linear term `z*c1` and the constants. This is a different structure from the ones tried so far, but it keeps three plaintext multiplies, so it may not beat attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext s = zz * z;
  Ciphertext q = s * c2;
  Ciphertext l = z * c1;
  Ciphertext r = q + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```
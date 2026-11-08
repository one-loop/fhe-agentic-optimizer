Idea: use the s2·w2 form from attempt 4, which has no T4 construction, but add the plaintext constants (c0, c4) early at the shallow level 2 alongside c1·z. The plaintext encodings and adds then happen at cheaper levels, and the product only adds ciphertext terms at the end.

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
  Ciphertext lowz = z * c1 + c0;
  lowz = lowz - c4;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext prod = s2 * (w + w);
  Ciphertext mid = t2 * c2 - a;
  mid = mid - a;
  mid = mid - a;
  Ciphertext result = prod + (mid + lowz);
  result.set_output("y");
}
```
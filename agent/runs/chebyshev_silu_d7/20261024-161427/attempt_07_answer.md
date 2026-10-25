Idea: add c0 to the c4·t4 term, which sits at an intermediate level, rather than to the final sum or the level-1 term. This changes when the plaintext is encoded and aligned, with the same multiplication structure.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext s2 = s + s;
  Ciphertext t4 = s2 - one;
  Ciphertext q = t4 + t4 - one;
  Ciphertext w = q * c6 + c2;
  Ciphertext r = t2 * w;
  Ciphertext mid = t4 * c4 + c0;
  Ciphertext result = z * c1 + mid;
  result = result + r;
  result.set_output("y");
}
```
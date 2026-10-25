Idea: add c0 to the level-1 term z·c1 first, where the ciphertext has more levels left, instead of adding it at the end. Plaintext encoding at that point may be cheaper, and it could change the alignment steps.

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
  Ciphertext low = z * c1 + c0;
  Ciphertext result = low + t4 * c4;
  result = result + r;
  result.set_output("y");
}
```
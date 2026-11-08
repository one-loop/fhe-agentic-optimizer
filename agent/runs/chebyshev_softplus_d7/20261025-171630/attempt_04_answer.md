Idea: same circuit as attempt 2, but with the sum grouped so that terms at the same level meet first. t2·c2 − a is built at level 3, then c1·z is added, then r at level 4. This should reduce level and scale alignment work.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext q = a + a + c4;
  Ciphertext r = t4 * q;
  Ciphertext low = t2 * c2 - a;
  low = low + c0;
  Ciphertext mid = low + z * c1;
  Ciphertext result = mid + r;
  result.set_output("y");
}
```
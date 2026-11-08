Idea: this is attempt 5 with c0 added to the shallow c1·z term. The add happens at a lower level, where it is cheaper, and the encoding is paid on a smaller ciphertext.

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
  Ciphertext mid = r + low;
  Ciphertext odd = z * c1 + c0;
  Ciphertext result = mid + odd;
  result.set_output("y");
}
```
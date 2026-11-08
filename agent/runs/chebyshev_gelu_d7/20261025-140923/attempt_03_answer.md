Idea: keep the structure, but double q before the final multiplication (q2 = q + q, then m2 = q2·b) instead of doubling the product afterwards. This removes one ciphertext add from the dependent path and, because q2 is available early, the doubling no longer sits behind the multiplication. The ct-ct multiplication count (3) is the minimum for a degree-6 polynomial in z.

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
  Ciphertext s = square(z);
  Ciphertext u = s + s - one;
  Ciphertext q = square(u);
  Ciphertext q2 = q + q;
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext m2 = q2 * b;
  Ciphertext r = z * c1 + u * c2;
  r = r - (a2 + a);
  r = r + m2;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```
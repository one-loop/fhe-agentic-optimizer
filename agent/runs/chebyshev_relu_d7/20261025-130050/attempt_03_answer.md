Idea: cut the depth to 4 by moving the c6 multiplication onto t, which is available early and runs in parallel with s. This costs one extra plaintext multiplication (c2·t) but removes a level from the critical path.

y = c0 + c1·z + c4·(2s − 1) + (c6·t)·(4s − 3) + c2·t

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
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext s2 = s + s;
  Ciphertext t4 = s2 - one;
  Ciphertext h = s2 + s2 - one - one - one;
  Ciphertext tc6 = t * c6;
  Ciphertext result = tc6 * h + t4 * c4 + t * c2 + z * c1 + c0;
  result.set_output("y");
}
```
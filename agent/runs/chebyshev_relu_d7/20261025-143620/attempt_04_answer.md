This version removes one plaintext add. Instead of building `T4 = q + q - one` and multiplying, I use `e·T4 = (2e)·q - e` with `e = 2·c6·T2 + c4`. The `one` subtraction on `T4` then becomes a cheap ciphertext subtract (~0.5 ms instead of ~2.5 ms for the encode). This gives a different circuit with the same multiplication count and depth.

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
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext e = a + a + c4;
  Ciphertext f = e + e;
  Ciphertext p = f * q - e;
  Ciphertext u = t2 * c2;
  Ciphertext w = z * c1;
  Ciphertext lower = u - a;
  Ciphertext result = p + lower;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```
This is a small variation: it adds c0 early at the lowest level, where the plaintext add is cheapest, and keeps the factored shared terms from attempt 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  auto dbl = [](const Ciphertext &c) { return c + c; };
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext zu = z * u;
  Ciphertext zv = zu * u;
  Ciphertext zw = zv * u;

  Ciphertext z2 = dbl(z);
  Ciphertext zu4 = dbl(dbl(zu));
  Ciphertext zu8 = dbl(zu4);
  Ciphertext n = zu4 - z;
  Ciphertext n5 = dbl(dbl(n)) + n;
  Ciphertext m = zu8 - z;
  Ciphertext m7 = dbl(dbl(dbl(m))) - m;

  Ciphertext zv7 = dbl(dbl(dbl(zv))) - zv;
  Ciphertext s = dbl(dbl(zw)) - zv7;
  Ciphertext s16 = dbl(dbl(dbl(dbl(s))));
  Ciphertext zv16 = dbl(dbl(dbl(dbl(zv))));

  Ciphertext t3 = n - z2;
  Ciphertext t5 = zv16 - n5;
  Ciphertext t7 = s16 + m7;

  Ciphertext r = z * c1 + c0;
  r = r + t3 * c3;
  r = r + t5 * c5;
  r = r + t7 * c7;
  r.set_output("y");
}
```
This version keeps the same 4 ct-ct chain but cuts the ciphertext adds by factoring shared sub-expressions. With n = 4zu − z and m = 8zu − z:
- T3 = n − 2z
- T5 = 16zv − 5n
- T7 = 16(4zw − 7zv) + 7m

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

  Ciphertext r = z * c1;
  r = r + t3 * c3;
  r = r + t5 * c5;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```
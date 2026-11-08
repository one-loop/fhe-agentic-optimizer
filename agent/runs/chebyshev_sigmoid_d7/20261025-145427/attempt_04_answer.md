This version stays in the power basis for z. It computes z·u, z·u² and z·u³ as one chain, which needs only 4 ct-ct multiplies, and builds T3, T5 and T7 from them with integer multiples done by ciphertext additions. The four coefficient multiplies then land at distinct levels, so no scale mismatches should occur. Depth drops to 6.

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
  Ciphertext z4 = dbl(z2);
  Ciphertext z8 = dbl(z4);

  Ciphertext zu2 = dbl(zu);
  Ciphertext zu4 = dbl(zu2);
  Ciphertext zu8 = dbl(zu4);
  Ciphertext zu64 = dbl(dbl(dbl(zu8)));

  Ciphertext e = dbl(dbl(dbl(dbl(zv))));       // 16 zv
  Ciphertext e8 = dbl(dbl(dbl(e)));            // 128 zv
  Ciphertext zw64 = dbl(dbl(dbl(dbl(dbl(dbl(zw))))));

  Ciphertext t3 = zu4 - (z + z2);
  Ciphertext zu20 = zu4 + dbl(dbl(zu4));
  Ciphertext t5 = e - zu20 + (z4 + z);
  Ciphertext zu56 = zu64 - zu8;
  Ciphertext zv112 = e8 - e;
  Ciphertext t7 = zw64 - zv112 + zu56 - (z8 - z);

  Ciphertext r = z * c1;
  r = r + t3 * c3;
  r = r + t5 * c5;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```
Idea: S is only ever used as 4·S, so I form R4 = 4·R with cheap additions at level 2 and multiply T2·R4 directly. That removes the doublings on the deeper-level product, which are slightly more expensive. The multiplication structure of the best kernel is unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext zc3d = zc3 + zc3;
  Ciphertext zc5d = zc5 + zc5;
  Ciphertext zc7d = zc7 + zc7;
  Ciphertext R = zc7d + c6;
  Ciphertext R2 = R + R;
  Ciphertext R4 = R2 + R2;
  Ciphertext S4 = T2 * R4;
  Ciphertext w2 = zc5d - zc7d;
  Ciphertext g2 = w2 + c4;
  Ciphertext g = g2 + g2;
  Ciphertext P = T2 * (g + S4);
  Ciphertext U = (((zc3d + c2) - (zc5d + c6)) - R) - R;
  Ciphertext F = T2 * (U + P);
  Ciphertext rem = (((zc1 + zc7) - (zc5 + zc3)) + c0) - c4;
  Ciphertext r = rem + F;
  r.set_output("y");
}
```
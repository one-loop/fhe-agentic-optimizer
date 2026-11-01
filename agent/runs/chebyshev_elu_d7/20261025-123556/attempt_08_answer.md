Attempts 6 and 7 gave the identical error (0.0569), which points to the common part: a `(ct − one)` subexpression multiplied by a plaintext coefficient. I suspect CHEHAB distributes that multiply into a plaintext-times-plaintext product, but I haven't confirmed it. This final kernel avoids the pattern.

- Every ciphertext polynomial fed to a coefficient multiply is built from ciphertext arithmetic only: 2v, 4v² − 2v, 8v³ − 4v² − 4v, 2v², and 4v³ − 3v.
- All constants are added or subtracted afterwards as plain `ct ± coefficient` operations, exactly as in attempt 5: +c1 − c3 − c5 + c7 in B, and +c0 − c4 in E.
- Plaintext multiplies drop from 11 to 7, and the terms still land at distinct levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext v = u + u - one;
  Ciphertext v2 = square(v);
  Ciphertext v3 = v * v2;

  Ciphertext vv = v + v;
  Ciphertext v2x2 = v2 + v2;
  Ciphertext v2x4 = v2x2 + v2x2;
  Ciphertext v3x2 = v3 + v3;
  Ciphertext v3x4 = v3x2 + v3x2;
  Ciphertext v3x8 = v3x4 + v3x4;

  Ciphertext P5 = v2x4 - vv;
  Ciphertext P7 = v3x8 - v2x4 - vv;
  Ciphertext Cc = v3x4 - vv - v;

  Ciphertext B = vv * c3;
  B = B + P5 * c5;
  B = B + P7 * c7;
  B = B + c1;
  B = B - c3;
  B = B - c5;
  B = B + c7;

  Ciphertext E = v * c2;
  E = E + v2x2 * c4;
  E = E + Cc * c6;
  E = E + c0;
  E = E - c4;

  Ciphertext zB = z * B;
  Ciphertext result = zB + E;
  result.set_output("y");
}
```
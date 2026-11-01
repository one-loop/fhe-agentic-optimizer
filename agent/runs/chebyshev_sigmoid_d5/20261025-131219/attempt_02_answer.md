The new idea is to multiply by the plaintext coefficients only at the end, which cuts the plaintext multiplications from 6 to 4 with the same 3 ciphertext multiplications.

Write w = 4z² and let zw = z·w. Then the output is

y = c0 + c1·z + c3·(zw − 3z) + c5·(zw·w − 5·zw + 5z)

The constants 3 and 5 are built from additions. The only plaintext multiplications are x·alpha, c1·z, c3·A and c5·B, where A and B are the two bracketed ciphertext terms.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw * w;
  Ciphertext z2 = z + z;
  Ciphertext z3 = z2 + z;
  Ciphertext z5 = (z2 + z2) + z;
  Ciphertext zw2x = zw + zw;
  Ciphertext zw5 = (zw2x + zw2x) + zw;
  Ciphertext A = zw - z3;
  Ciphertext B = (zw2 - zw5) + z5;
  Ciphertext y = (z * c1 + A * c3) + B * c5;
  y = y + c0;
  y.set_output("y");
}
```
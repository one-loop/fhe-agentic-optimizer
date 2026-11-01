**Idea:** Use Paterson-Stockmeyer-style Chebyshev splitting with the identity T_{4+j} = 2·T4·T_j − T_{4−j}. This gives p(z) = low(z) + T4(z)·r(z), where low has the terms c0..c4 plus the low-order parts of c5..c7, and r = 2c5·z + 2c6·T2 + 2c7·T3. The ciphertext-ciphertext multiplications drop from 6 to 4 (z², z·T2, T2², and T4·r). The rest become ciphertext-plaintext multiplications, which are cheaper. Chebyshev coefficients are used directly, so no plaintext-plaintext arithmetic is needed, and doubling is written as `q + q`.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // Chebyshev T2, T3, T4 via ciphertext products
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // Low part: c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3 + c4 T4
  Ciphertext a1 = z * c1 - z * c7;
  Ciphertext a2 = T2 * c2 - T2 * c6;
  Ciphertext a3 = T3 * c3 - T3 * c5;
  Ciphertext a4 = T4 * c4;
  Ciphertext low = a1 + a2 + a3 + a4 + c0;

  // High part: T4 * r, r = 2 c5 T1 + 2 c6 T2 + 2 c7 T3
  Ciphertext r1 = z * c5;
  Ciphertext r2 = T2 * c6;
  Ciphertext r3 = T3 * c7;
  Ciphertext r = (r1 + r1) + (r2 + r2) + (r3 + r3);
  Ciphertext high = T4 * r;

  Ciphertext y = low + high;
  y.set_output("y");
}
```
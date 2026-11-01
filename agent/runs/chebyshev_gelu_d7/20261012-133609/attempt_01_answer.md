The baseline computes T3, T5 and T7 only to build the higher terms, and it multiplies by z once per recurrence step. Since only T1, T2, T4 and T6 are needed, I use the doubling identities T4 = 2·T2² − 1 and T6 = 2·T3² − 1, with T3 = 2·z·T2 − z. This cuts ciphertext-ciphertext multiplies from 5 to 4 and keeps the multiplicative depth at 5 for the polynomial part, with the coefficient multiplies on top. The `alpha` scaling stays as the first plaintext multiply.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T3 = 2 z T2 - z
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;

  // T4 = 2 T2^2 - 1
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  // T6 = 2 T3^2 - 1
  Ciphertext t3sq = t3 * t3;
  Ciphertext t6 = t3sq + t3sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext y = z * c1;
  y = y + t2 * c2;
  y = y + t4 * c4;
  y = y + t6 * c6;
  y = y + c0;
  y.set_output("y");
}
```
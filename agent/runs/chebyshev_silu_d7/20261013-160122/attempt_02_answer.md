Attempt 1 cut the ciphertext multiplies from 5 to 3, but depth is still 5 and the top term needs an extra plaintext multiply. The change below keeps 3 ciphertext multiplies and reduces the critical depth to 4 and the plaintext multiplies to 4.

The identity used is c6·T6 + c4·T4 = T4·(2·c6·T2 + c4) − c6·T2. So the degree-4 and degree-6 terms share one ciphertext multiply, whose inputs are at level 3 and whose output is at level 4. The c6·T2 term is computed at level 3 and subtracted there, next to the c2·T2 term, so those two share a multiplication history at the same level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext q = T2 * T2;
  Ciphertext T4 = q + q - one;

  // c6*T6 + c4*T4 = T4 * (2 c6 T2 + c4) - c6 T2
  Plaintext c6("c6");
  Plaintext c4("c4");
  Ciphertext v = T2 * c6;
  Ciphertext u = v + v + c4;
  Ciphertext m = T4 * u;

  Plaintext c2("c2");
  Ciphertext w = T2 * c2;
  Ciphertext wv = w - v;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  Ciphertext result = wv + term1;
  result = result + m;
  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```
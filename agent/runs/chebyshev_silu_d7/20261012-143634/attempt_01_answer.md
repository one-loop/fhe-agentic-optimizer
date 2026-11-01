The baseline uses 5 ciphertext-ciphertext multiplies. I rebuild the Chebyshev basis with only three of them: z², T4 = 2·T2² − 1, and T6 = 2·T2·T4 − T2, which uses the identity T_{m+n} = 2T_mT_n − T_{|m−n|}. The four coefficient terms are then plaintext multiplies, and the final sum adds terms at different levels, so CHEHAB can align them cheaply. Total multiplies drop from 10 to 8, and multiplicative depth drops from 7 to 5.

```cpp
// Chebyshev silu degree 7 on [-4,4].
// T2 = 2z^2-1, T4 = 2*T2^2-1, T6 = 2*T2*T4 - T2 (identity T_{m+n}=2T_mT_n-T_{|m-n|}).
// Only 3 ciphertext-ciphertext multiplies (z^2, T2^2, T2*T4) and 5 plaintext multiplies.
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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;

  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;

  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;

  Ciphertext term1 = z * c1;
  Ciphertext term2 = t2 * c2;
  Ciphertext term4 = t4 * c4;
  Ciphertext term6 = t6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
```
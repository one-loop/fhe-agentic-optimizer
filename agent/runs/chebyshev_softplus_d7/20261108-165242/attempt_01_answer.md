The approach is to evaluate only the even terms through s = T2, using the identity T6 = T2·(2·T4 − 1) so that no T3, T5 or T7 is needed.

- Let z = α·x, u = z², and s = T2 = 2u − 1.
- Then T4 = 2s² − 1, and c4·T4 + c6·T6 = T4·(c4 + 2·c6·s) − c6·s.
- The result is y = T4·d + (c2·s − c6·s) + c1·z + c0, where d = 2·(c6·s) + c4.

This uses 3 ciphertext-ciphertext multiplications (z², s², T4·d) instead of 5, and the depth drops from 7 to 4. The product c6·s is shared between d and the c2/c6 term, and c4 and c0 are added as plaintext adds without any plaintext-plaintext arithmetic. I have not compiled or run it, so the correctness and timing are untested.

```cpp
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
  Ciphertext s = u + u - one;
  Ciphertext q = s * s;
  Ciphertext t4 = q + q - one;
  Ciphertext c6s = s * c6;
  Ciphertext d = c6s + c6s + c4;
  Ciphertext c2s = s * c2;
  Ciphertext lin = z * c1;
  Ciphertext main_term = t4 * d;
  Ciphertext low = c2s - c6s;
  Ciphertext result = main_term + low + lin + c0;
  result.set_output("y");
}
```
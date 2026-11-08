The idea is to build the odd Chebyshev polynomials with the product identities T_{m+n} = 2·T_m·T_n − T_{|m−n|} instead of the linear recurrence. This brings the depth from 8 to about 5 and removes some multiplications.

- z = x·alpha, then w = z·z and T2 = 2w − 1.
- T3 = 4·z·w − 3z. The constants 4 and 3 are built from repeated additions, since numeric constants are not allowed.
- T4 = 2·T2² − 1, T5 = 2·T3·T2 − z, and T7 = 2·T4·T3 − z.
- y = c1·z + c3·T3 + c5·T5 + c7·T7 + c0.

This uses 5 ciphertext-ciphertext multiplications and 5 plaintext multiplications (alpha and c1, c3, c5, c7). I have not compiled or timed it, so the savings are expected, not measured.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw + zw;
  Ciphertext t3 = zw2 + zw2 - z - z - z;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext p5 = t3 * t2;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```
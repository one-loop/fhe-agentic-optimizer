The polynomial is odd, so I build the Chebyshev basis with the product identities T_{m+n} = 2·T_m·T_n − T_|m−n| instead of the linear recurrence. This keeps the depth low:

- z = x·alpha at level 1, and u = z² gives T2 = 2u − 1 at level 2.
- T3 = 2·z·T2 − z and T4 = 2·T2² − 1 are both at level 3.
- T5 = 2·T2·T3 − z and T7 = 2·T3·T4 − z are both at level 4.
- The c_k multiplications land at level 5.

That is 5 ciphertext-ciphertext multiplications and 5 levels, against 6 multiplications and 8 levels in the baseline. I haven't compiled or run it, so the timing gain and correctness are untested.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result.set_output("y");
}
```
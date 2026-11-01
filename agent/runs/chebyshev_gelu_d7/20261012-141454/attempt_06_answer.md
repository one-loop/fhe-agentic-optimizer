I'm stopping here. The same computation has now measured 115.88–115.99 ms across attempts 1, 3, 4 and 5, so further resubmissions are just noise. The kernel below is attempt 3's, which was the fastest measured (115.88 ms, against a 152.48 ms baseline).

It needs only 3 ciphertext-ciphertext multiplications (z², T2², T4·T2), which is the minimum for a degree-6 Chebyshev basis, and 5 ciphertext-plaintext multiplications. Those can't be merged because plaintext-plaintext products are disallowed. Depth is 5, down from 7.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  // z = x/4 in [-1, 1]
  Ciphertext z = x * alpha;

  // T2 = 2z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2*T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // T6 = 2*T4*T2 - T2
  Ciphertext P = T4 * T2;
  Ciphertext T6 = P + P - T2;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;
  Plaintext c2("c2");
  Ciphertext term2 = T2 * c2;
  Plaintext c4("c4");
  Ciphertext term4 = T4 * c4;
  Plaintext c6("c6");
  Ciphertext term6 = T6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```
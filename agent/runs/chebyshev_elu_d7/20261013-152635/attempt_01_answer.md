## Idea

The baseline spends 6 ciphertext-ciphertext multiplies and a depth-8 chain on the Chebyshev recurrence. I use the Chebyshev product identity T_{m+n} = 2T_mT_n − T_{|m−n|} to split the degree-7 polynomial around T4:

- T5 = 2T4·T1 − T3, T6 = 2T4·T2 − T2, T7 = 2T4·T3 − T1
- So p = T4·q + r, where q = c4 + 2c5·T1 + 2c6·T2 + 2c7·T3 and r = c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3.

Only four ciphertext multiplies remain: z², z·T2, T2², and the final T4·q. The rest are plaintext-scalar multiplies and additions. To avoid plaintext-plaintext arithmetic and numeric constants, the factors of 2 are written as self-additions, and the differences of coefficients are applied as separate ciphertext products that are then subtracted. The expected multiplicative depth is about 5.

```cpp
// Chebyshev elu degree 7 on [-4, 4] using T_{m+n} = 2 T_m T_n - T_{|m-n|}.
// p = T4 * q + r, with
//   q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
//   r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
// Only 4 ciphertext-ciphertext multiplies, depth about 5.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // Baby steps: T2, T3, T4
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
  Ciphertext a1 = z * c5;
  a1 = a1 + a1;
  Ciphertext b2 = T2 * c6;
  Ciphertext b3 = T3 * c7;
  b3 = b3 + b3;
  b2 = b2 + b2;
  Ciphertext q = a1 + b2 + b3;
  q = q + c4;

  // r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
  Ciphertext r1 = z * c1 - z * c7;
  Ciphertext r2 = T2 * c2 - T2 * c6;
  Ciphertext r3 = T3 * c3 - T3 * c5;
  Ciphertext r = r1 + r2 + r3;
  r = r + c0;

  Ciphertext prod = T4 * q;
  Ciphertext result = prod + r;
  result.set_output("y");
}
```
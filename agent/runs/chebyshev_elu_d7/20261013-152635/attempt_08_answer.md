I'm stopping here rather than spending the last attempt. The kernel has been resubmitted several times with the same operation counts, and timings vary from about 335 to 429 ms. The differences are within noise, so I can't claim a real improvement over the baseline beyond that range.

## Idea

The kernel uses the Chebyshev product identity to split the degree-7 polynomial around T4: p = T4·q + r, with q and r linear in T1, T2, T3. That needs 4 ciphertext multiplies (z², z·T2, T2², and T4·q), which I believe is the floor for degree 7 under these rules. Plaintext-scalar multiplies can't be folded further because the factors of 2 and the coefficient differences would require plaintext-plaintext arithmetic. Depth is 5 instead of the baseline's 8, which makes the later plaintext operations cheaper.

This is the same kernel as attempt 3, the best measured run at 335.27 ms.

```cpp
// Chebyshev elu degree 7 on [-4, 4] using T_{m+n} = 2 T_m T_n - T_{|m-n|}.
// p = T4 * q + r, with
//   q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
//   r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
// Four ciphertext-ciphertext multiplies, depth 5.
// Terms are grouped by multiplication history so same-level adds need fewer alignments.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // Baby steps
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // Depth-1 linear terms (z times scalar), grouped
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext zc1 = z * c1;
  Ciphertext q1 = zc5 + zc5;
  Ciphertext r1 = zc1 - zc7;

  // Depth-2 linear terms (T2 times scalar), grouped
  Ciphertext T2c6 = T2 * c6;
  Ciphertext T2c2 = T2 * c2;
  Ciphertext q2 = T2c6 + T2c6;
  Ciphertext r2 = T2c2 - T2c6;

  // Depth-3 linear terms (T3 times scalar), grouped
  Ciphertext T3c7 = T3 * c7;
  Ciphertext T3c3 = T3 * c3;
  Ciphertext T3c5 = T3 * c5;
  Ciphertext q3 = T3c7 + T3c7;
  Ciphertext r3 = T3c3 - T3c5;

  // q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
  Ciphertext q = q1 + q2 + q3;
  q = q + c4;

  // r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
  Ciphertext r = r1 + r2 + r3;
  r = r + c0;

  Ciphertext prod = T4 * q;
  Ciphertext result = prod + r;
  result.set_output("y");
}
```
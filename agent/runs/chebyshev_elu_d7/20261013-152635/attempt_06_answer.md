## Idea

Attempt 5 (337.6 ms) matches attempt 3 within noise, with identical operation counts. The kernel is at a plateau for this algorithm: 4 ciphertext multiplies is the floor for degree 7, and the remaining plaintext multiplies and alignments follow from the rules, not from the code shape. I tried a few reorganizations and none moved the count of `match_scale` or `ct_pt_mul` operations. Since attempt 3 measured best (335.27 ms, error 1.15e-07), I am resubmitting that kernel as the final answer.

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
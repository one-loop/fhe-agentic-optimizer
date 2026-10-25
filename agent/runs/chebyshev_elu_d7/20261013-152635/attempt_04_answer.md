## Idea

Attempt 3 (335 ms) is the best so far, with the same operation counts as attempts 1 and 2. The only redundant work I can see is `T2 * c6`, which is computed twice (once for `b2` in q and once for `r2` in r). Computing it once with an explicit variable removes one plaintext multiply (~6 ms) without changing the math or the depth. I expect a small gain, not a large one. The four ciphertext multiplies are the floor for this degree and structure.

```cpp
// Chebyshev elu degree 7 on [-4, 4] using T_{m+n} = 2 T_m T_n - T_{|m-n|}.
// p = T4 * q + r, with
//   q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
//   r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
// Four ciphertext-ciphertext multiplies, depth 5.
// T2 * c6 is computed once and shared between q and r.

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

  // Shared products
  Ciphertext T2c6 = T2 * c6;

  // q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
  Ciphertext a1 = z * c5;
  a1 = a1 + a1;
  Ciphertext b2 = T2c6 + T2c6;
  Ciphertext b3 = T3 * c7;
  b3 = b3 + b3;
  Ciphertext q = a1 + b2 + b3;
  q = q + c4;

  // r = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
  Ciphertext r1 = z * c1 - z * c7;
  Ciphertext r2 = T2 * c2 - T2c6;
  Ciphertext r3 = T3 * c3 - T3 * c5;
  Ciphertext r = r1 + r2 + r3;
  r = r + c0;

  Ciphertext prod = T4 * q;
  Ciphertext result = prod + r;
  result.set_output("y");
}
```
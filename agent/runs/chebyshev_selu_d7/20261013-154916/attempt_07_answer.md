The best kernel is still attempt 4 at 346.39 ms. Attempts 5 and 6 are the same kernel within run-to-run noise (346.74 ms and 353.48 ms), so the spread is measurement jitter rather than a real difference. The structure has no further saving I can justify: 4 ciphertext-ciphertext multiplies and 9 ciphertext-plaintext multiplies, with c5, c6 and c7 each needed in both the lower and giant-step parts. I'm returning the attempt-4 kernel as the final answer.

```cpp
// Chebyshev-basis baby-step/giant-step evaluation of the degree-7 series in z = alpha*x.
// T1 = z, T2 = 2 z^2 - 1, T3 = 2 z T2 - z, T4 = 2 T2^2 - 1.
// T_{4+j} = 2 T4 T_j - T_{4-j} gives
//   p = c0 + c4 T4 + sum_{j=1..3} c_j T_j + sum_{j=1..3} c_{4+j} (2 T4 T_j - T_{4-j})
//     = [c0 + c1 T1 + c2 T2 + c3 T3 - c7 T1 - c6 T2 - c5 T3] + T4 * (2 (c5 T1 + c6 T2 + c7 T3) + c4).

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

  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  Ciphertext T3 = T2 * z;
  T3 = T3 + T3 - z;

  Ciphertext T4 = T2 * T2;
  T4 = T4 + T4 - one;

  // A = c0 + c1 T1 + c2 T2 + c3 T3 - c7 T1 - c6 T2 - c5 T3
  Ciphertext A = z * c1;
  A = A + T2 * c2;
  A = A + T3 * c3;
  A = A - z * c7;
  A = A - T2 * c6;
  A = A - T3 * c5;
  A = A + c0;

  // B = c5 T1 + c6 T2 + c7 T3, then 2B + c4
  Ciphertext B = z * c5;
  B = B + T2 * c6;
  B = B + T3 * c7;
  Ciphertext B2 = B + B;
  B2 = B2 + c4;

  Ciphertext Q = T4 * B2;
  Ciphertext y = A + Q;
  y.set_output("y");
}
```
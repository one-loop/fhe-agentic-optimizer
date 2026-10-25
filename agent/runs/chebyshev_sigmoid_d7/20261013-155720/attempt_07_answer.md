## Idea

Attempt 6 is the fastest so far at 286.5 ms, and it matches the attempt-5 kernel. Results across identical kernels vary by about 25 ms, so further gains from reshuffling are within noise. I resubmit this kernel unchanged as the final answer.

```cpp
// Chebyshev sigmoid degree 7 on [-4,4].
//   T5 = 2*T2*T3 - T1,  T7 = 2*T3*T4 - T1
//   p = c0 + (c1 - c5 - c7)*z + T3*(2*(c5*T2 + c7*T4) + c3)
// Four ciphertext-ciphertext multiplies in total.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;            // z = x/4 in [-1,1]

  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;       // T2 = 2z^2 - 1

  Ciphertext z2 = z + z;
  Ciphertext T3 = z2 * T2 - z;         // T3 = 2z*T2 - z

  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;   // T4 = 2*T2^2 - 1

  Ciphertext A = T2 * c5 + T4 * c7;    // c5*T2 + c7*T4
  Ciphertext A2 = A + A;               // 2*(c5*T2 + c7*T4)
  Ciphertext G = A2 + c3;              // 2*(...) + c3
  Ciphertext B = T3 * G;               // T3 * G

  Ciphertext lin = z * c1 - z * c5 - z * c7;   // (c1 - c5 - c7)*z

  Ciphertext result = B + lin + c0;
  result.set_output("y");
}
```
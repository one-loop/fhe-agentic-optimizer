The last four attempts all measured 110.6 to 110.75 ms, so this structure has plateaued. Attempt 2 remains the fastest at 110.58 ms, and I did not find a rule-compliant change that beats it. I resubmit that kernel.

```cpp
// Chebyshev silu degree 7 on [-4,4].
// T2 = 2z^2-1, T4 = 2*T2^2-1.
// y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6, with T6 = 2 T2 T4 - T2:
//   c4 T4 + c6 T6 = T4*(c4 + 2 c6 T2) - c6 T2
// 3 ct-ct muls (z^2, T2^2, T4*r) and 4 pt muls (alpha, c6*T2, c2*T2, c1*z).
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
  Ciphertext t2 = u + u - one;

  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;

  Ciphertext q = t2 * c6;
  Ciphertext r = q + q + c4;
  Ciphertext m = t4 * r;

  Ciphertext term_t2 = t2 * c2;
  Ciphertext term_z = z * c1;

  Ciphertext result = m - q;
  result = result + term_t2;
  result = result + term_z;
  result = result + c0;
  result.set_output("y");
}
```
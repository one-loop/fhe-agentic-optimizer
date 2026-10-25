I'm keeping the attempt 2 kernel as the answer. It is the best measured run at 214.25 ms, against a baseline of 303.26 ms. The later resubmissions of the same kernel measured between 214.99 and 217.28 ms, so the difference is run-to-run noise.

The kernel uses three ciphertext multiplies, which is the minimum for a degree-6 polynomial, and four plaintext multiplies. I don't see a structural change that would clearly beat this, so I'm not spending the remaining attempts on it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext q = T2 * T2;
  Ciphertext T4 = q + q - one;

  // c6*T6 + c4*T4 = T4 * (2 c6 T2 + c4) - c6 T2
  Plaintext c6("c6");
  Plaintext c4("c4");
  Ciphertext v = T2 * c6;
  Ciphertext u = v + v + c4;
  Ciphertext m = T4 * u;

  Plaintext c2("c2");
  Ciphertext w = T2 * c2;
  Ciphertext wv = w - v;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  Ciphertext result = wv + term1;
  result = result + m;
  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```
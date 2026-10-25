Attempt 5 ran the same kernel again and measured 230.05 ms, which is within noise of the best (229.62 ms, attempt 2). I'm not changing the kernel. The operation mix (3 ciphertext multiplies, 5 plaintext multiplies, depth 5) is the minimum I found for a degree-6 Chebyshev evaluation without plaintext-plaintext products, so further attempts would only chase timing noise.

Final kernel (attempt 2):

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
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // T6 = 2 T4 T2 - T2
  Ciphertext T4T2 = T4 * T2;
  Ciphertext T6 = T4T2 + T4T2 - T2;

  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Plaintext c0("c0");

  Ciphertext r1 = z * c1;
  Ciphertext r2 = T2 * c2;
  Ciphertext r4 = T4 * c4;
  Ciphertext r6 = T6 * c6;

  Ciphertext result = r1 + r2;
  result = result + r4;
  result = result + r6;
  result = result + c0;
  result.set_output("y");
}
```
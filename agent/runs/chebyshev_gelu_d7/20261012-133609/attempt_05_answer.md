Attempt 4 is the same kernel and measured 116.11 ms, so the spread between runs is about ±1 ms and the best time is still attempt 2 at 115.00 ms. I considered further restructuring, such as folding `alpha` into the coefficients, but any such change needs plaintext-plaintext arithmetic, which is not allowed. The three ciphertext multiplies and five plaintext multiplies are what remain, so I'm submitting the attempt 2 kernel as the final answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  // T6 = 2 T2 T4 - T2
  Ciphertext q = t2 * t4;
  Ciphertext t6 = q + q - t2;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext y = z * c1;
  y = y + t2 * c2;
  y = y + t4 * c4;
  y = y + t6 * c6;
  y = y + c0;
  y.set_output("y");
}
```
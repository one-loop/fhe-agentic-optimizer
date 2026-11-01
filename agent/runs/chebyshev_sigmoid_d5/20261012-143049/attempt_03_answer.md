**Idea:** I considered whether the remaining cost can be cut further and found no meaningful gain. Degree 5 needs at least 3 ciphertext-ciphertext multiplications, and the kernel uses exactly 3. The plaintext multiplications for alpha, c3 and c5 are needed to apply the scalar coefficients, and c3 and c5 cannot be merged because combining plaintext values is not allowed. Eliminating the alpha multiplication would mean scaling by 1/16 elsewhere, which again needs a plaintext multiply. The constant offsets (+5, −3) could be folded with doubling tricks, but that would save only a few milliseconds and risks correctness. Attempts 2 and 3 are within noise of each other (107.66 vs 107.78 ms), so this kernel is at a practical optimum for this factorization. I resubmit it as the final kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  // z = x / 4 in [-1, 1]
  Ciphertext z = x * alpha;

  // u = z^2, u2 = z^4
  Ciphertext u = z * z;
  Ciphertext u2 = u * u;

  // 4u, 8u, 16u via doublings (no level cost)
  Ciphertext u4 = u + u;
  u4 = u4 + u4;
  Ciphertext u8 = u4 + u4;
  Ciphertext u16 = u8 + u8;

  // 16u^2 via 4 doublings of u2
  Ciphertext v2 = u2 + u2;
  v2 = v2 + v2;
  v2 = v2 + v2;
  v2 = v2 + v2;

  // w = 16u^2 - 20u + 5
  Ciphertext w = v2 - (u16 + u4);
  w = w + one;
  w = w + one;
  w = w + one;
  w = w + one;
  w = w + one;

  // A = 4u - 3
  Ciphertext A = u4 - one;
  A = A - one;
  A = A - one;

  // Q = c1 + c3 * A + c5 * w
  Ciphertext q = A * c3 + w * c5;
  q = q + c1;

  // y = c0 + z * Q
  Ciphertext y = z * q;
  y = y + c0;
  y.set_output("y");
}
```
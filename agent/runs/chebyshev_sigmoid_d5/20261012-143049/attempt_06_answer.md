I'm not proposing a further change. The last five attempts measured 107.66–107.89 ms, so the spread is noise and this factorization has no meaningful headroom left. The best measured kernel is attempt 2 (107.66 ms, against a 129.13 ms baseline, about 17% faster). It uses 3 ciphertext multiplications (the minimum for degree 5), 3 plaintext multiplications, and depth 5, and it is correct to about 3e-8.

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
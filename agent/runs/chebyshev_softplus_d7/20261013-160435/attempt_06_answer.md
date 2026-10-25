**Idea:** The best kernel so far is attempt 4 (218.66 ms, error 1.1e-7). Attempt 5 is the same computation and measured about the same, within noise. I'm returning the attempt 4 kernel unchanged. It uses three ciphertext multiplications, four plaintext multiplications, and depth 4.

```cpp
// Chebyshev softplus degree 7 on [-4,4] (z = x/4).
// y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6
// Rewritten using T6 = 2 T2 T4 - T2:
//   c4 T4 + c6 T6 = T4 * (c4 + 2 c6 T2) - c6 T2
// so y = c0 + c1 z + (c2 - c6) T2 + T4 * (c4 + 2 c6 T2).

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z_square = z * z;
  Ciphertext t2 = z_square + z_square - one;

  // T4 = 2 T2^2 - 1
  Ciphertext t2_square = t2 * t2;
  Ciphertext t4 = t2_square + t2_square - one;

  Plaintext c6("c6");
  Ciphertext u = t2 * c6;          // c6 * T2
  Plaintext c4("c4");
  Ciphertext inner = (u + u) + c4; // c4 + 2 c6 T2
  Ciphertext prod = t4 * inner;    // T4 * (c4 + 2 c6 T2)

  Plaintext c2("c2");
  Ciphertext t2c2 = t2 * c2;
  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  Ciphertext result = prod + t2c2;
  result = result - u;
  result = result + term1;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```
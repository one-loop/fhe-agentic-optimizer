I'm keeping the attempt-1 kernel as the final answer. Attempt 7 ran the same code and measured 41.45 ms, in the same noise band as every other run (41.32–41.48 ms), so there is nothing new to try.

The cost is dominated by one ciphertext×ciphertext multiply (about 30 ms), which a degree-2 polynomial in x cannot avoid. The rest is two scalar plaintext multiplies, three plaintext additions and one mod switch, about 11 ms in total. Every alternative I considered either costs more than it saves or breaks a CKKS rule.

Result: correct on all cases, max error about 1e-7 against the 1e-6 tolerance, and about 41.3 ms against the 53.4 ms baseline (roughly 23% faster).

```cpp
// Horner form of the degree-2 Chebyshev approximation in z = x/4:
//   y = c0 - c2 + z * (2*c2*z + c1)
// One ciphertext-ciphertext multiply, two scalar plaintext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");

  Ciphertext z = x * alpha;          // z in [-1, 1], depth 1
  Ciphertext a = z * c2;             // c2 * z, depth 2
  Ciphertext b = a + a;              // 2 * c2 * z (same level, no alignment)
  Ciphertext inner = b + c1;         // 2 c2 z + c1, depth 2
  Ciphertext p = inner * z;          // z * (2 c2 z + c1), depth 3
  Ciphertext q = p - c2;             // subtract c2
  Ciphertext y = q + c0;             // add c0
  y.set_output("y");
}
```
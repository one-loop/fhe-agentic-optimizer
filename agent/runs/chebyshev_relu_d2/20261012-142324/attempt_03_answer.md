Attempt 2 was the same kernel as attempt 1 and measured 41.39 ms against 41.32 ms, which is run-to-run noise. I considered three remaining tweaks and none looks worth trying:

- Removing the mod switch by aligning `z` would need an extra scalar multiply, which costs more than the roughly 1 ms it saves.
- Folding `c1` into the multiply would need another ciphertext multiply, at about 30 ms.
- Reordering the plaintext additions only moves about 2.5 ms of encoding cost, and the additions already run at the lowest levels.

The ciphertext-ciphertext multiply (about 30 ms) is the floor for this degree-2 polynomial, so I'm resubmitting the attempt-1 kernel as the final answer.

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
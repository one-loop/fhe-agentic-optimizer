This attempt removes one level, going from 5 to 4. Attempt 6 computes y = z·B + c0 with B = n·(m·c5 + c3) − c5 + c1, and its chain is z → z² → m·c5 → n·inner → z·B.

Here I distribute z instead:

y = c0 + (z·n)·(m·c5 + c3) + (z·c1 − z·c5)

- **z·n** (ct-ct, L3) is computed in parallel with inner = m·c5 + c3 (L3).
- **The product** (z·n)·inner lands at L4 instead of L5.
- **The constant term** (c1 − c5) becomes two cheap scalar plaintext multiplications on z (L2), replacing the plaintext adds of c1 and −c5.

There are still 3 ct-ct multiplications, and every operation runs one level shallower.

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

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = (z2 + z2) - one;   // 2z^2 - 1
  Ciphertext m = t2 + t2;            // 4z^2 - 2
  Ciphertext n = m - one;            // 4z^2 - 3

  Ciphertext inner = m * c5 + c3;    // c5 (4z^2 - 2) + c3
  Ciphertext zn = z * n;             // T3
  Ciphertext lowz = z * c1 - z * c5; // (c1 - c5) z
  Ciphertext result = (zn * inner + lowz) + c0;
  result.set_output("y");
}
```
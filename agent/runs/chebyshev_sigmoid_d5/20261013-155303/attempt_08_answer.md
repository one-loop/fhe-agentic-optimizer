I'm submitting attempt 5 as the final kernel. It is the fastest correct version I measured, at 227.54 ms against the baseline's 257.56 ms. Attempts 6 and 7 are the same computation and differ only by timing noise (229.05 and 228.34 ms).

**Idea:** Three ciphertext multiplies (z², z³, z⁵) are the minimum for a degree-5 odd polynomial. The c3 and c5 terms are built as ciphertext linear combinations (4z³ − 3z and 16z⁵ − 20z³ + 5z) using only additions and doublings, so each coefficient needs one plaintext multiply. That leaves 4 plaintext multiplies in total (alpha, c1, c3, c5). Removing the remaining level alignments would cost extra plaintext multiplies, so I didn't pursue it further.

```cpp
// y = c0 + c1 z + c3 A + c5 B,  z = x/4
// A = 4 z^3 - 3 z,  B = 16 z^5 - 20 z^3 + 5 z   (built with additions only)

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext z3 = z2 * z;
  Ciphertext z5 = z3 * z2;

  // 3z and 5z, 4z
  Ciphertext z2x = z + z;
  Ciphertext z3x = z2x + z;
  Ciphertext z4x = z2x + z2x;
  Ciphertext z5x = z4x + z;

  // 4 z^3, 16 z^3, 20 z^3
  Ciphertext z3x4 = z3 + z3;
  z3x4 = z3x4 + z3x4;
  Ciphertext z3x8 = z3x4 + z3x4;
  Ciphertext z3x16 = z3x8 + z3x8;
  Ciphertext z3x20 = z3x16 + z3x4;

  // 16 z^5
  Ciphertext z5x2 = z5 + z5;
  Ciphertext z5x4 = z5x2 + z5x2;
  Ciphertext z5x8 = z5x4 + z5x4;
  Ciphertext z5x16 = z5x8 + z5x8;

  // A = 4z^3 - 3z,  B = 16z^5 - 20z^3 + 5z
  Ciphertext A = z3x4 - z3x;
  Ciphertext B = z5x16 - z3x20;
  B = B + z5x;

  Ciphertext result = z * c1;
  result = result + A * c3;
  result = result + B * c5;
  result = result + c0;
  result.set_output("y");
}
```
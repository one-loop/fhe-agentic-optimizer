**Idea:** This is the fastest kernel so far (227.54 ms). It keeps the structure that worked: 3 ciphertext multiplies (the minimum for z⁵), and 4 plaintext multiplies (alpha, c1, c3, c5). The c3 and c5 terms are formed as ciphertext linear combinations (4z³ − 3z and 16z⁵ − 20z³ + 5z) using only additions and doublings, so each coefficient is applied with one plaintext multiply. Further gains look marginal: the remaining alignments come from mixing the shallow z term with the deeper z³ and z⁵ terms, and removing them would need extra plaintext multiplies. I resubmit this kernel as the final answer.

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
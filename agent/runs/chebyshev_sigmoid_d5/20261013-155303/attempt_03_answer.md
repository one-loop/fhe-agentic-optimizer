**Idea:** Attempt 2 is the fastest kernel so far (239.91 ms, versus 257.56 ms for the baseline). Its cost is dominated by the three ciphertext multiplies, which is already the minimum for a degree-5 odd polynomial (z², z³, z⁵). Each alternative I considered either adds a fourth ciphertext multiply (for example z⁴ = z²·z² for a Horner form) or needs plaintext-plaintext or constant arithmetic, which the rules disallow. Scalar multiples like 4, 16, 20 are built from ciphertext doublings, which are cheaper than plaintext multiplies. So I resubmit the attempt 2 structure unchanged, since I don't have a change I'm confident would beat it.

```cpp
// y = c0 + c1 z + c3 (4z^3 - 3z) + c5 (16z^5 - 20z^3 + 5z),  z = x/4
// Shared structure: z*(c1 - 3c3 + 5c5) + z^3*(4c3 - 20c5) + z^5*(16c5)

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

  // Linear-in-z part: (c1 - 3c3 + 5c5) * z.
  Ciphertext p1 = z * c1;
  Ciphertext p3 = z * c3;
  Ciphertext p5 = z * c5;
  Ciphertext p3x3 = p3 + p3;
  p3x3 = p3x3 + p3;
  Ciphertext p5x4 = p5 + p5;
  p5x4 = p5x4 + p5x4;
  Ciphertext p5x5 = p5 + p5x4;
  Ciphertext lin = p1 + p5x5;
  lin = lin - p3x3;

  // z^3 part: (4c3 - 20c5) * z^3.
  Ciphertext q3 = z3 * c3;
  Ciphertext q5 = z3 * c5;
  Ciphertext a4 = q3 + q3;
  a4 = a4 + a4;
  Ciphertext q4 = q5 + q5;
  q4 = q4 + q4;
  Ciphertext q16 = q4 + q4;
  q16 = q16 + q16;
  Ciphertext t20 = q16 + q4;
  Ciphertext cub = a4 - t20;

  // z^5 part: 16 c5 * z^5.
  Ciphertext r5 = z5 * c5;
  Ciphertext b16 = r5 + r5;
  b16 = b16 + b16;
  b16 = b16 + b16;
  b16 = b16 + b16;

  Ciphertext result = lin + cub;
  result = result + b16;
  result = result + c0;
  result.set_output("y");
}
```
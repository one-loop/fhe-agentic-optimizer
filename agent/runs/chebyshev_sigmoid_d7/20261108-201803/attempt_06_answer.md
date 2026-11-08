The multiplication counts (5 ciphertext-ciphertext, 5 plaintext) and depth 4 are at their floor. I also ruled out the power basis: its integer coefficients go up to 112, and they would amplify the noise past the 1e-6 tolerance.

The remaining overhead is how the cross-level sums get aligned. So this keeps attempt 5's lean circuit, with no s and no T4 and a single "one", and changes only the alignment order:
- The level-2 linear group is first added to the level-3 group (2·T2·r − 4p). That needs one short alignment from level 2 to 3.
- The combined level-3 sum then meets the level-4 term once. This avoids aligning the level-2 term all the way to level 4.

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
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext t2s = t2 * t2;

  Ciphertext p = t2 * z7;
  Ciphertext q = p + p + (z5 - z7);
  Ciphertext g = t2s * q;
  Ciphertext g2 = g + g;
  Ciphertext g4 = g2 + g2;

  Ciphertext r = z3 - z5;
  Ciphertext a = t2 * r;
  Ciphertext p2 = p + p;
  Ciphertext lin = z1 + z7 - z3 - z5;
  Ciphertext mid = (a + a - p2 - p2) + lin;

  Ciphertext result = g4 + mid + c0;
  result.set_output("y");
}
```
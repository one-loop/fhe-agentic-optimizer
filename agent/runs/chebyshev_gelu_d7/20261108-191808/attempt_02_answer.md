The current circuit is close to minimal: 3 ct-ct multiplications, 4 plaintext multiplications and 4 levels. The degree-6 even part needs two levels above z², and z itself costs one. The remaining overhead is the 2 scale alignments.

This attempt keeps the algebra the same and only regroups the sum:

- **Low-level terms first:** c0, c1·z (level 2) and (c2 − c6)·u (level 3) are added together.
- **Then the high term:** the combined low part meets the level-4 term T4·w once, at the end.
- **What it might save:** CHEHAB may then align c1·z with a cheap mod switch instead of a multiply-plain alignment.
- **Small change:** the constant c0 is attached to the lowest-level term.

If CHEHAB normalizes the sum order, this regrouping produces the same circuit as attempt 1 and cannot win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext mid = u * c2 - c6u;       // (c2 - c6) u, level 3
  Ciphertext low = mid + lin;          // level 3
  Ciphertext uu = u * u;               // level 3
  Ciphertext t4 = uu + uu - one;       // T4, level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext high = t4 * w;            // level 4
  Ciphertext result = high + low;
  result.set_output("y");
}
```
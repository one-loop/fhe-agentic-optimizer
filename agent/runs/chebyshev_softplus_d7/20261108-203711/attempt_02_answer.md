The level count is already at the minimum for this structure. A cubic in w = z² with folded coefficients needs 4 levels. So this attempt targets the 3 scale alignments instead.

In attempt 1, three lower-level terms were each aligned separately up to level 4:
- `−u` at level 3
- `c2·T2` at level 3
- `c1·z` at level 2

Here I group the low-level terms first:
- `c2·T2 − c6·T2` is combined at level 3. Both are T2 times a scalar plaintext, so their scales match.
- `c1·z` is then added to that sum.
- The whole low part is aligned once against the level-4 product `T4·inner`.

This should cut the number of alignments and rescales.

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
  Ciphertext zz = z * z;               // level 2
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = t2 * t2;            // level 3
  Ciphertext t4 = t2s + t2s - one;     // level 3

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext inner = u + u + c4;       // level 3
  Ciphertext prod = t4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext mid = term2 - u;          // level 3, same scale pattern
  Ciphertext term1 = z * c1;           // level 2
  Ciphertext low = mid + term1;        // level 3

  Ciphertext result = prod + low + c0;
  result.set_output("y");
}
```
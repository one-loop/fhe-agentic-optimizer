The circuit is already at the minimum I can see: 3 ct-ct multiplications, 4 plaintext multiplications and 4 levels, so this attempt is a low-confidence experiment on where alignment happens.

c1·z is computed at level 2. In attempt 3 it was first aligned to level 3 (joined with the low part), and then that sum was aligned again to level 4.

This time:
- The level-3 low part (c2·T2 − 3u − c4) is combined on its own.
- c0 is added to the level-4 product, where the ciphertext carries fewer primes, so the encoding is cheaper.
- term1 is added last, so CHEHAB aligns it directly from level 2 to level 4, possibly with a cheap mod switch.

If CHEHAB normalizes this back to the attempt-3 circuit, it cannot win.

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
  Ciphertext w4 = t2s + t2s;           // level 3, = T4 + 1

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext u2 = u + u;
  Ciphertext inner = u2 + c4;          // level 3
  Ciphertext prod = w4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext low3 = term2 - u - inner; // level 3
  Ciphertext term1 = z * c1;           // level 2

  Ciphertext high = prod + c0;         // level 4
  Ciphertext result = (high + low3) + term1;
  result.set_output("y");
}
```
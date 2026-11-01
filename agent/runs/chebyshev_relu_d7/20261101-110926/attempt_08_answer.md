This is the last attempt. The arithmetic stays as in attempt 5: depth 4, 3 ciphertext–ciphertext multiplies (2 as `square`), 4 scalar multiplies and 3 plaintext encodes.

What changes is the placement of the constant c0. It is now added to the odd term c1·z at level 2, the shallowest point, instead of to the level-3 low part. The combined (c1·z + c0) is then merged with (c2 − 3c6)u − c4 at level 3 before the final level-4 add. This changes where CHEHAB encodes c0 and how it aligns the shallow sum. The circuit differs from earlier attempts, but expect a gain of only a few milliseconds at most, likely short of the 3% threshold.

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

  Ciphertext z = x * alpha;          // level 1
  Ciphertext odd = z * c1 + c0;      // level 2: c1 z + c0
  Ciphertext zz = square(z);         // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = square(u);         // level 3

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4

  Ciphertext low = cu - v - q;       // level 3: (c2-3c6)u - c4
  Ciphertext low2 = low + odd;
  Ciphertext r = u2 * q;             // level 4
  Ciphertext high = r + r;           // 2 u^2 q

  Ciphertext y = low2 + high;
  y.set_output("y");
}
```
Let y = c0 + c1·z + c2·(2z² − 1), with z = α·x.

Rewrite it as y = (c0 − c2) + c1·α·x + 2·c2·α²·x².

Plaintext-plaintext arithmetic is not allowed, so I can't precompute the combined coefficients. I can still reduce the work by factoring:

y = c0 − c2 + z·(c1 + 2·c2·z)

Let w = c2·z (one plaintext multiplication). Then 2·c2·z = w + w, and u = (w + w) + c1 is a plaintext add. Then z·u is a ciphertext multiplication, and y = z·u + (c0 − c2).

The c0 − c2 term needs a plaintext-plaintext subtraction, which is not allowed. Instead I compute (z·u + c0) − c2, which uses two plaintext adds.

Count the levels and operations:
- z = x·α: 1 ct-pt multiplication, level 1.
- w = z·c2: 1 ct-pt multiplication, level 2.
- u = w + w + c1: level 2.
- z·u: 1 ct-ct multiplication, level 3.

That is 2 ct-pt multiplications and 1 ct-ct multiplication, against 3 ct-pt multiplications and 1 ct-ct in the baseline. Z is at level 1 while u is at level 2, so CHEHAB handles the alignment.

An alternative is to start from x directly: w = x·c2 at level 1, and a·w with a = α gives the level-2 term. Take u' = (w + w)·α + c1, which is level 2. Then y = (x·α)·u' + c0 − c2.
- x·α: ct-pt multiplication, level 1.
- x·c2: ct-pt multiplication, level 1.
- (w + w)·α: ct-pt multiplication, level 2.

That is 3 ct-pt multiplications, so it is worse. I'll keep the first version.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext u = (w + w) + c1;
  Ciphertext p = z * u;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
```
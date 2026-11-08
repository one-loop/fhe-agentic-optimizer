Attempt 4 works because it saves a level. This version also removes two top-level plaintext multiplies, `X1 = s3c1·x` and `x·half`, together with the long mod-switch chains that carry them down to level 8–9.

The degree-1 term and the +0.5 term are merged into a single product, `x·(s3c1·t + half)`. Here `s3c1·t` is a plaintext multiply at level 7, where it is cheap, followed by a plaintext add; the product with x then sits at level 8. The ciphertext-multiplication count stays the same.

That product and the `(X3·t)·u` term are both at level 8 but have different histories. So I add each of them separately to the level-9 high part, where the level difference lets CHEHAB align them for free. This only works if CHEHAB keeps my order of additions and does not distribute the x back out of `x·(s3c1·t + half)`; attempt 4's grouping survived, so I am relying on that.

```cpp
Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_base(const Ciphertext &t, int stage)
{
  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext low = t * coeff(stage, 1) + (t * coeff(stage, 3)) * t2;
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * t2;
  return low + high * t4;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext X3 = x * coeff(3, 3);
  Ciphertext X5 = x * coeff(3, 5);
  Ciphertext X7 = x * coeff(3, 7);
  Plaintext half("half");

  Ciphertext t = stage_base(x, 1);
  t = stage_base(t, 2);

  Ciphertext u = t * t;                      // L7
  Ciphertext u2 = u * u;                     // L8
  Ciphertext B = t * coeff(3, 1) + half;     // L7
  Ciphertext A0 = x * B;                     // L8
  Ciphertext A3 = (X3 * t) * u;              // L8
  Ciphertext H = X5 * t + (X7 * t) * u;      // L8
  Ciphertext high = H * u2;                  // L9
  Ciphertext y = (high + A0) + A3;
  y.set_output("y");
}
```
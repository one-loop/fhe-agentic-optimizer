Let z = αx, so y = c0 + c1·z + c2·(2z² − 1) = c0 − c2 + c1·z + 2c2·z².

Rewriting in terms of x, with a = c1·α and b = 2·c2·α²:
y = (c0 − c2) + a·x + b·x²
  = (c0 − c2) + x·(a + b·x)

This needs one ct-ct multiplication and one ct-pt multiplication, if the plaintext combinations can be formed. Plaintext-plaintext arithmetic is not allowed, though, so I can't compute a or b directly. Instead, the plaintext multiplications go on the ciphertext side:

- u = x·c1 (ct-pt)
- Using z = x·alpha, z² = z·z, and w = z·c2 gives (w + w)·z = 2c2·z², which is ct-ct.

So:
- z = x·alpha (pt mul, level 1)
- w = z·c2 (pt mul, level 2)
- q = w·z (ct-ct, level 3), which equals c2·z²
- t = z·c1 (level 2)
- y = q + q + t + c0 − c2, where I still need c0 − c2 without plaintext arithmetic. I can fold the constant in as (−c2 + c0) = c0 − c2 by adding c0 to the ciphertext and subtracting c2 from it. These are plaintext additions of about 2.5 ms each, so that's fine.

That is 3 pt muls and 1 ct-ct, the same as the baseline. A better option is to avoid the first alpha multiplication:

- p = x·c2 (pt, level 1)
- r = x·alpha (pt, level 1)
- s = p·r (ct-ct, level 2), which equals c2·alpha·x²; this is not yet the full coefficient.

Since I need 2·c2·α²·x², I use z·z with z = x·alpha:
- z = x·alpha, then w = z·c2 and q = w·z, which is the same as above.

Alternative: y = c0 − c2 + z·(c1 + 2·c2·z). Computing c1 + 2·c2·z needs 1 pt mul (c2·z) plus an add of c1, then 1 ct-ct with z. That is z (1 mul), c2·z (1 mul), and the ct-ct, for 3 multiplications and depth 3. This is the same count but a different structure.

To cut depth, I compute z = x·alpha and v = x·c2 in parallel at level 1. Then v·z = c2·alpha·x² is a ct-ct at level 2. I still need an extra factor of 2·alpha, which is the issue. Using v' = x·c1 gives the linear term at level 1, so the only problem is the square coefficient. Taking the alpha factor into account:

- z = x·alpha (level 1)
- m = z·c2 (level 2)
- m2 = m + m
- q = m2·z (level 3)

This stays at depth 3. The two-level option below loses nothing and uses one fewer multiplication:

- z = x·alpha
- u = z·c1 (level 2), and independently m = z·c2
- q = (m+m)·z

That is the same count again, so I'll go with the restructured form that shares the work: y = c0 − c2 + z·(c1 + (c2+c2)·z). Here z·(c2+c2) uses a pt mul and the c1 add is a plaintext add. Then the final ct-ct multiplication removes the separate c1·z term, for a total of 2 pt muls and 1 ct-ct.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext m2 = m + m;
  Ciphertext s = m2 + c1;
  Ciphertext p = s * z;
  Ciphertext r = p + c0;
  Ciphertext y = r - c2;
  y.set_output("y");
}
```
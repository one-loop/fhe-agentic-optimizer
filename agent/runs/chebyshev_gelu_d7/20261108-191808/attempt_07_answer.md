These attempts have converged on the same counts: 4 levels, 3 ct-ct multiplications, 4 plaintext multiplications, 2 alignments. So this attempt combines the two placements that measured fastest, which haven't been tried together:

- **From attempt 3:** the level-2 term c1·z joins the level-3 term mid first, so the low part reaches level 4 through one aligned sum.
- **From attempt 4:** the plaintext add of c0 is done last, on the level-4 ciphertext, where it carries the fewest primes.
- **Kept throughout:** T4 is never formed, because T4·w = 2(u²·w) − w. That saves one plaintext add.

The expected gain is small. With the algebra fixed, this ordering is only likely to beat attempt 1 by about the margin attempts 3 and 4 showed (1–2%).

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
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = u * u;               // level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext p = uu * w;               // level 4
  Ciphertext lin = z * c1;             // level 2
  Ciphertext low = u * c2 - c6u - w + lin;   // level 3
  Ciphertext result = (p + p + low) + c0;
  result.set_output("y");
}
```
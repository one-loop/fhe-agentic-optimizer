I don't see a cheaper algebraic form. Every variant I tried either adds plaintext adds or multiplications, or needs plaintext-plaintext arithmetic, which isn't allowed.

So this attempt only combines the two scheduling ideas that each measured close to the best:
- `square()` for z² and T2².
- Adding c0 to the level-4 product and adding c1·z last.

The algebra is unchanged:
- T2 = 2z² − 1.
- w4 = 2·T2² = T4 + 1.
- y = w4·(2c6·T2 + c4) + c2·T2 − 3c6·T2 − c4 + c1·z + c0.

Expect a tie with attempt 3, within noise.

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
  Ciphertext zz = square(z);           // level 2
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = square(t2);         // level 3
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
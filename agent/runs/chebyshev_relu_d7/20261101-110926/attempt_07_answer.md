The structure is the same as attempt 5: 3 ciphertext–ciphertext multiplies (2 as `square`), 4 scalar multiplies and depth 4. One small change: the factor of 2 on the top product is applied before the multiply instead of after it.

| Step | Before | Now |
|---|---|---|
| Doubling | r + r at level 4 | q2 = q + q at level 3 |
| Top product | u²·q, then doubled | u²·q2 = 2u²·q directly |
| Rest of the even part | unchanged | − q + (c2 − 3c6)u + c0 |

The odd term is still merged into the level-3 part before the final add.

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
  Ciphertext odd = z * c1;           // level 2
  Ciphertext zz = square(z);         // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = square(u);         // level 3

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4
  Ciphertext q2 = q + q;             // 4 c6 u + 2 c4

  Ciphertext low = (cu - v - q) + c0; // level 3: (c2-3c6)u - c4 + c0
  Ciphertext low2 = low + odd;
  Ciphertext high = u2 * q2;         // level 4: 2 u^2 q

  Ciphertext y = low2 + high;
  y.set_output("y");
}
```
The 3 ciphertext–ciphertext and 4 ciphertext–plaintext multiplies look close to minimal here. The two coefficients c2 and c6 must each multiply u separately, and c1 and alpha each need their own multiply. So this attempt targets the alignment and add structure instead.

The shallow terms are combined first:

- the odd term c1·z (level 2),
- the linear term (c2 − c6)·u (level 3),
- the constant c0, added while the operand is still at level 3, where encoding is cheaper.

Only then does the result meet the level-4 product T4·q. Each alignment then happens once, at the shallowest possible point. The constant also no longer rides on the deepest ciphertext.

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
  Ciphertext zz = z * z;             // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = u * u;             // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4(z)

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4

  Ciphertext low = (cu - v) + c0;    // (c2 - c6) u + c0, level 3
  Ciphertext low2 = low + odd;       // align odd once, at level 3
  Ciphertext high = t4 * q;          // level 4

  Ciphertext y = low2 + high;
  y.set_output("y");
}
```
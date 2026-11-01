The multiplication count (3 ciphertext–ciphertext, 4 ciphertext–plaintext) and the depth of 4 appear minimal for this polynomial:

- **Depth:** alpha costs one level, and a degree-6 even part needs 3 more ciphertext–ciphertext levels.
- **Plaintext multiplies:** the independent coefficients c1, c2 and c6 each need their own multiply.

So this attempt changes only the kind of multiplication. Both z·z and u·u are self-products, so they are emitted as `square()`. SEAL's squaring uses 3 polynomial products instead of 4 and yields a smaller intermediate. That makes each of those two multiplications cheaper, while the rest of the attempt-3 structure stays the same.

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
  Ciphertext t4 = u2 + u2 - one;     // T4(z)

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4

  Ciphertext low = (cu - v) + c0;    // (c2 - c6) u + c0
  Ciphertext low2 = low + odd;
  Ciphertext high = t4 * q;          // level 4

  Ciphertext y = low2 + high;
  y.set_output("y");
}
```
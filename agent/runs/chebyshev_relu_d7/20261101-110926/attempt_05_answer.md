One plaintext encoding can be saved. Since T4 = 2u² − 1, the product T4·q equals 2u²·q − q. The `− q` term is the same q ciphertext (q = 2v + c4) already computed for the product. It sits at level 3 and has the same scale pattern as the other level-3 terms, so subtracting it costs only a ciphertext subtraction with no new alignment.

This removes the second `− one` encoding (the one for T4). Attempt 1 had instead written an explicit `− c4`, so the circuit now encodes only 3 plaintext constants: one, c4 and c0. The kernel keeps `square()` for z² and u², and keeps the shallow terms grouped before they meet the level-4 product.

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

  Ciphertext low = (cu - v - q) + c0; // level 3: (c2-3c6)u - c4 + c0
  Ciphertext low2 = low + odd;
  Ciphertext r = u2 * q;             // level 4
  Ciphertext high = r + r;           // 2 u^2 q

  Ciphertext y = low2 + high;
  y.set_output("y");
}
```
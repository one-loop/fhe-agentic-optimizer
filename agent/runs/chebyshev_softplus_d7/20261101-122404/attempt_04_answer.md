Same 3-multiply, 4-plaintext-multiply, depth-4 algebra as before. Here I use the explicit T4 form: `y = T4·s − c6·T2 + c2·T2 + c1·z + c0`, with `s = 2c6·T2 + c4`.

The changes:
- Both self-products use `square()`.
- c0 is folded into the level-3 partial sum.
- `c1·z` is aligned once onto that level-3 sum before the final add with the deepest product. The aim is to drop a mod-switch or scale alignment relative to attempt 1.

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

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;          // T2
  Ciphertext u = square(w);
  Ciphertext t4 = u + u - one;           // T4
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext l1 = z * c1;                // level 2
  Ciphertext low = (b - a + c0) + l1;    // level 3
  Ciphertext r4 = t4 * s;                // c4 T4 + c6 (T6 + T2)
  Ciphertext r = r4 + low;
  r.set_output("y");
}
```
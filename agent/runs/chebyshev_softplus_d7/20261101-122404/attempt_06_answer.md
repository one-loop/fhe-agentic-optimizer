This keeps the 3 ct-ct multiplies, 3 plaintext multiplies and the c1 = 2 trick.

What changes is the shape of the sum, borrowed from attempt 4, which gave the shortest total depth (9, 4) with no mod switches:
- The even part is written as `T4·s + (c2·T2 − c6·T2 + c0)`, with T4 built explicitly.
- The level-1 term `z + z` is added last, directly onto the deepest result, so CHEHAB aligns it in one step.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
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
  Ciphertext low = b - a + c0;           // level 3
  Ciphertext r4 = t4 * s;                // c4 T4 + c6 (T6 + T2)
  Ciphertext l1 = z + z;                 // c1*T1 with c1 = 2
  Ciphertext r = (r4 + low) + l1;
  r.set_output("y");
}
```
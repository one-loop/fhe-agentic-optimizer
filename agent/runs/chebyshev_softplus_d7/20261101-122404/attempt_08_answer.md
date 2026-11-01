Last attempt: I'm combining the two fastest circuits so far.

- **From attempt 5 (best):** the even part is evaluated as `u·p + q`, where `u = T2²`, `p = 2(c4 + 2c6·T2)` and `q = c2·T2 − 3c6·T2 − c4`. The plaintext-side doubling `p = s + s` is also attempt 5's.
- **From attempt 7:** T2 is built as `z·(z+z) − 1`. This reuses the odd term `z + z = c1·T1` (with c1 = 2), so no separate square-and-double is needed.
- **Grouping change:** c0 and `z + z` are summed first, then added to the level-3 part. The aim is to change, and hopefully reduce, how CHEHAB aligns the low-level terms before the final add.

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
  Ciphertext l1 = z + z;                 // c1*T1 with c1 = 2
  Ciphertext v = z * l1;                 // 2 z^2
  Ciphertext w = v - one;                // T2
  Ciphertext u = square(w);              // T2^2
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext p = s + s;
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext lo = l1 + c0;               // level 1
  Ciphertext q = (b - a - s) + lo;       // level 3
  Ciphertext r4 = u * p;                 // 2 u s
  Ciphertext r = r4 + q;
  r.set_output("y");
}
```
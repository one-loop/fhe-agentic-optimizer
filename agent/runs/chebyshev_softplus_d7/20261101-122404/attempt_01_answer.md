Idea: evaluate only the even Chebyshev terms through `w = T2`, using `T4 = 2w² − 1` and `T6 = T2·(2T4 − 1)`. Then factor the c4/c6 part:

`c4·T4 + c6·T6 = T4·(c4 + 2·c6·w) − c6·w`.

**Expected circuit:**
- 3 ciphertext-ciphertext multiplies: z², w², and T4·s. The baseline has 5.
- 4 plaintext multiplies: alpha, c1, c2, c6. The baseline has 5.
- c4 enters as a plaintext add, not a multiply.
- The deepest path should use 4 levels instead of 7.

**Scale check:**
- `c2·w` and `c6·w` meet at the same level with the same multiplication history, so they can be added.
- Every other addition joins terms at different levels, which CHEHAB aligns for free.

I haven't compiled or run this yet, so the depth, counts and error are what I expect, not measured.

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
  Ciphertext z2 = z * z;
  Ciphertext w = z2 + z2 - one;          // T2
  Ciphertext u = w * w;
  Ciphertext t4 = u + u - one;           // T4
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext r3 = b - a;
  Ciphertext r4 = t4 * s;                // c4 T4 + c6 (T6 + T2)
  Ciphertext r = r4 + r3;
  Ciphertext l1 = z * c1;
  r = r + l1;
  r = r + c0;
  r.set_output("y");
}
```
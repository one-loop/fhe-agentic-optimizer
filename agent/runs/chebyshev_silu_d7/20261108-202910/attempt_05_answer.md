I can't find a cheaper algebraic form. Under these rules, 3 ciphertext multiplications and 4 plaintext multiplications (alpha, c1, c2, c6) look like the floor, because c1, c2 and c6 are independent and no plaintext-plaintext arithmetic is allowed. That leaves the two scale alignments as the only target.

This attempt keeps the cheaper square-based core from attempt 4 and changes only where the linear term lands:

- The T2-level terms (c2·T2 − c6·T2 − w) are kept together at level 3.
- The linear part (c1·z + c0) is added to the level-4 product separately, instead of going through the level-3 sum.

The aim is to replace one scale alignment with a cheap mod switch.

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
  Ciphertext u = square(z);            // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = square(t2);           // level 3 (T4 = 2s - 1)

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext sw = s * w;               // level 4

  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext mid = c2t2 - c6t2 - w;    // level 3

  Ciphertext lin = z * c1 + c0;        // level 2

  Ciphertext high = sw + sw + mid;
  Ciphertext result = high + lin;
  result.set_output("y");
}
```
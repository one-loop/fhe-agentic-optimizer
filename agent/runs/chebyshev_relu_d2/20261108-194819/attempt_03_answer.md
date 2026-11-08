I believe attempt 1 is already optimal for this polynomial:

- **Depth:** 3 levels is the minimum, because the term α²·c2·x² has five factors.
- **Multiplies:** at least 1 ciphertext×ciphertext multiply is needed for the square, and at least 2 ciphertext×plaintext multiplies to bring in α and c2.
- **Adds:** the 4 adds are the minimum, since plaintext-plaintext arithmetic is not allowed.
- **Alignment:** the only overhead left is a ~1 ms mod switch.

There is one structurally different circuit worth timing: move the α factor onto the x branch, so the ciphertext×ciphertext multiply uses raw `x`. That gives y = x·(2·c2·α²·x + c1·α) + c0 − c2. This needs α applied twice on the deep branch, but the shallow operand is `x` at level 0 instead of a rescaled ciphertext. The alignment then happens on a fresh input, which may change CHEHAB's alignment choice.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // alpha x
  Ciphertext w = z * c2;             // c2 alpha x
  Ciphertext v = w + w + c1;         // 2 c2 z + c1
  Ciphertext p = x * v;              // x (2 c2 z + c1)
  Ciphertext q = p * alpha;          // z (2 c2 z + c1)
  Ciphertext y = q - c2 + c0;
  y.set_output("y");
}
```
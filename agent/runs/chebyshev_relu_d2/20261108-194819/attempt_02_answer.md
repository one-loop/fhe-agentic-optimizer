The current kernel already looks minimal. The depth of 3 is forced: α²·c2·x² has five factors, and five leaves cannot fit in a binary tree of depth 2. The circuit is already at 1 ciphertext×ciphertext multiply, 2 ciphertext×plaintext multiplies, 3 rescales and one ~1 ms mod switch.

The remaining overhead is the two plaintext adds, each costing about 2.5 ms of encoding:
- One add, of c1, happens one level lower, where it is cheaper.
- The constant c0 − c2 cannot be pre-combined, because plaintext-plaintext arithmetic is not allowed.

As a variant I fold the doubling before the plaintext multiply: v = (z+z)·c2 + c1. The operation counts are probably the same, so this may not register as a distinct circuit. I don't see a structurally cheaper circuit for this polynomial.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = (z + z) * c2 + c1;
  Ciphertext p = z * v;
  Ciphertext y = p - c2 + c0;
  y.set_output("y");
}
```
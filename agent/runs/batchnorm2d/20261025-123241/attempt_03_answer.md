I still have no cheaper circuit. Every variant I can build either reduces to the baseline after CHEHAB's simplification or adds operations:

- **Required work:** y = x*A + B needs one ct-pt multiply with its rescale, plus one plaintext add.
- **Folding B in:** this would need B/A, which is plaintext-plaintext arithmetic and isn't allowed.
- **Other structures:** extra ciphertext operations, such as a zero ciphertext built as x - x, are either simplified away or cost more.

I'm keeping the minimal kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```
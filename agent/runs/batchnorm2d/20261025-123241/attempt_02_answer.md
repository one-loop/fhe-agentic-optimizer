I don't see a cheaper circuit. The ct-pt multiply with its rescale and the plaintext add are the minimum for y = x*A + B:

- **Multiply:** A varies per slot, so the multiply can't be removed.
- **Add:** B can only enter as a plaintext add, since folding it into the multiply would need B/A, which is plaintext-plaintext arithmetic.
- **Rescale:** CHEHAB inserts it automatically, so I can't remove it.

Any other structure adds operations such as extra adds, multiplies or alignments, and those cost more. I'm resubmitting the minimal form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```
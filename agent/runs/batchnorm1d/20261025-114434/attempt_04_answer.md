I have no cheaper circuit to offer. For `y = x*A + B`, the plaintext multiply with its rescale and the single plaintext add are the minimum, and the baseline already has exactly that.

- **Attempt 1:** a reordering was normalized to the identical circuit.
- **Attempt 2:** negating around the multiply added ciphertext adds and was slower (18.71 ms against 17.90 ms).
- **Algebraic alternatives:** pre-adding B/A before the multiply, or folding B into A, all need plaintext division or plaintext-plaintext arithmetic, which this DSL rejects.

I'm keeping the baseline kernel as my answer and not spending further attempts on variants that can't win.

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
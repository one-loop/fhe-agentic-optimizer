I don't see a cheaper circuit than the baseline. Its one plaintext multiply (with rescale) and one plaintext add are the minimum for `y = x*A + B`:

- **Multiply:** A varies per slot, so the multiply by A and its rescale can't be dropped.
- **Add:** B has to be encoded and added once.
- **Rewrites:** every algebraic rewrite I can think of either adds operations or needs plaintext-plaintext arithmetic or constants, which CKKS in this DSL forbids.

The negation variant confirmed this: it added three extra ciphertext adds and was slower (18.71 ms against 17.90 ms).

I'm resubmitting the baseline form, which is the best circuit I have. CHEHAB will report it as identical to the baseline, so it can't win.

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
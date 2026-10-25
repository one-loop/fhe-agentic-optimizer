I have to correct my last message. I said the kernel used `-=` on a negated scalar, but it didn't. It used `y += s`, which is the same circuit as the baseline, and that is why it was rejected.

The baseline is already one add. Cheaper circuits don't exist, because the output must depend on both inputs and that takes at least one operation. Every other form I can write either adds operations or compiles to the identical circuit. Here is what I considered:

- **Subtraction form:** `-(-x0 - s)` adds two negations, so it is worse.
- **Doubling `x0`:** this adds operations and does not change the result.
- **Multiplying by `s`:** this computes a different function.

So I'm submitting the minimal single-add kernel again. It will probably be rejected as an identical circuit, and I don't expect further attempts to produce a faster one.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = s + x0;
  y.set_output("y");
}
```
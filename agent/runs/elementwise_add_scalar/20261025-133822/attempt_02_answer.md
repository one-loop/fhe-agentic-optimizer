The baseline is one ciphertext-plaintext add, which is already the minimum for this function. The circuit needs at least one operation to combine x0 and s, and any other form I can write either adds operations or compiles back to the same circuit.

Two alternatives I considered:
- **Add as a subtraction of a negated value.** Negating a plaintext is not allowed, because plaintext-plaintext arithmetic is forbidden, so this does not work.
- **Compute x0 + s through a multiplication.** This would add a level and a rescale, which is strictly slower.

I am not going to submit a rewrite that I expect to be slower or identical. The best kernel remains the baseline, and I am resubmitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
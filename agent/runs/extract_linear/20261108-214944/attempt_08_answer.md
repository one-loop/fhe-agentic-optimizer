This is the final attempt, and I'm submitting the baseline again. No correct circuit for this function is cheaper.

- **Multiply:** the mask is a runtime plaintext, so one plaintext multiply plus its rescale is unavoidable.
- **Rotation:** the output is shifted, and the DSL rejects rotating a plaintext (attempt 2). So one ciphertext rotation is unavoidable.
- **Order:** the baseline already rotates after the rescale, which is the cheaper order.

Any structurally different correct kernel would only add operations and run slower. The best result for this task is the baseline at 4.75 ms.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```
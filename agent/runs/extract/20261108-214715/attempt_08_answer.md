This is the last attempt, and I still can't propose a cheaper circuit. The baseline is the minimum for this task and should stand as the final result:

- **Masking:** one plaintext multiply with its rescale is the only way under CKKS to zero the slots outside the window.
- **Shifting:** one rotation moves the window to slot 0. It runs after the rescale, at the cheaper level.
- **Rotating first:** this would need a rotated plaintext mask, which CHEHAB rejects (attempt 2).

Every other correct circuit adds operations or levels. Final kernel:

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
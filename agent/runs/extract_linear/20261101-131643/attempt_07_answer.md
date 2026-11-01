Attempt 6 gave the same result. With 2 attempts left my assessment is unchanged: the baseline circuit is minimal for this task, and I cannot produce a correct kernel with a different and cheaper circuit.

- **Multiply:** one ct-pt multiply with its rescale is needed, because `mask` is an arbitrary runtime plaintext.
- **Rotation:** one rotation is needed to apply the shift. It already runs at the cheapest level.
- **Alternatives:** rotating the plaintext is rejected by CHEHAB (attempt 2), and rotating the ciphertext before the multiply gives wrong results. Anything else adds operations.

I'm returning the baseline as the final kernel. Further attempts can only repeat this circuit or be slower or incorrect.

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
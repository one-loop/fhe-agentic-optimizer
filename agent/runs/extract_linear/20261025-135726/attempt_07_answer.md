I have no cheaper circuit for this task. I've now resubmitted the baseline circuit three times (attempts 4–6), and it was identical each time. The only structurally different form, rotating the plaintext mask, was rejected by CHEHAB as plaintext-plaintext arithmetic.

The baseline needs one plaintext multiply with its rescale and one rotation. Neither can be removed, and the rotation already runs at the lowest level. I'm not using the remaining attempts on variants that can only match or exceed this cost, so the baseline stays the best kernel.

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
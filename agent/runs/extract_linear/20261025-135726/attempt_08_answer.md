I still have no cheaper circuit, and I've now resubmitted the same circuit four times (attempts 4–7). The baseline's one plaintext multiply with rescale and one rotation is the minimum for `rotate(input * mask)`. I'm keeping it as the final answer rather than risk a worse circuit on the last attempt.

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
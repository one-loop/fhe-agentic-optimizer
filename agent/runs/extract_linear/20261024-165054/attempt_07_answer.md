I have no further idea for a cheaper circuit, and I'm not going to submit a variant I expect to lose. Each of the last attempts (4, 5 and 6) was a resubmission of the baseline, so none could count and I shouldn't have spent them that way.

The baseline's one ct-pt multiply with rescale and one rotation are both required. Every alternative I can think of either adds operations or relies on an assumption about the mask that I can't check.

The best kernel stays the baseline at 59.04 ms:

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
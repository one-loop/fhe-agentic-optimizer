I'm submitting the baseline kernel as the final answer. Eight runs of this same operation sequence measured between 59.1 and 63.5 ms, so the spread is noise. The computation is one ciphertext-plaintext multiply followed by one rotation, and both are required: the rotation moves input slot i+offset to output slot i, and the mask must be applied elementwise. The one structural alternative I tried was rejected by the backend, and I have no legal way to reduce the operation count further.

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
The plaintext rotation was rejected, because CHEHAB treats it as a plaintext-plaintext operation. That leaves `(input * mask) << slots()/4` as the only correct form. Every other form I can think of adds operations:

- **Rotating before the multiply** moves the rotation to the top level, where it costs more, and the mask can't be rotated to match.
- **Splitting the rotation** adds key-switches.
- **Adding extra multiplies or alignments** only adds cost.

So I'm keeping the baseline as the best kernel. I can't find a cheaper circuit for this function, and I'm not spending further attempts on variants that CHEHAB would either reject or normalize back to this circuit.

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
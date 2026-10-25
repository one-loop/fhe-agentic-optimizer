The idea is to rotate the input first and apply the mask pre-rotated by the same offset. The output is the same as (input·mask) rotated, but the circuit has a different shape. If CHEHAB allows rotating a plaintext input, that rotation is done at encoding time and costs no ciphertext rotation. This may fail to compile, in which case I'll fall back.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext m2 = mask << offset;
  Ciphertext output = (input << offset) * m2;
  output.set_output("output");
}
```
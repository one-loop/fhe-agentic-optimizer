I'm returning the same kernel as final. This run measured 59.51 ms, and the timings across identical runs range from 59.1 to 63.5 ms, so the differences are noise. The computation needs one ciphertext-plaintext multiply and one rotation, and I found no legal reformulation that reduces either. The alternative ordering (rotate the input, then multiply by a rotated mask) was rejected because it requires a plaintext rotation.

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
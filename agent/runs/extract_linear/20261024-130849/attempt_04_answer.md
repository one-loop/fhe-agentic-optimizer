This is the same kernel as the baseline. I can't find a structure with fewer operations: the function needs one mask multiply and one rotation, and CHEHAB already performs the rotation after the rescale, at the cheaper lower level. The 59.23 ms result is within noise of the baseline.

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
I have no faster kernel to offer. The best measured time is 59.23 ms (attempt 3), which is the baseline kernel and within noise of the original 59.42 ms. This final submission is that same kernel again.

The function needs one plaintext multiply and one rotation, and I found no way to cut either. The only alternative I tried, rotating the mask, was rejected by CHEHAB.

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
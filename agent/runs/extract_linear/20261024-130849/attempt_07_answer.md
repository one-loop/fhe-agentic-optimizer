The kernel is unchanged again. I don't have a faster structure: the function needs one plaintext multiply and one rotation, and every attempt landed within noise of the 59.4 ms baseline.

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
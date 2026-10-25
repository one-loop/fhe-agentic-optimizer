There is no cheaper correct circuit, and I'm not going to submit a wrong or slower one just to differ from the baseline. The baseline already uses the minimum: one plaintext multiply, one rescale and one rotation done at the reduced level.

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
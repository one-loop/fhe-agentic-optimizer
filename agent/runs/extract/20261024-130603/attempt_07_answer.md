This is the same kernel again. The 59.61 ms result is a new best only by noise, since every attempt so far has run the identical program. The kernel already has the minimum operation set for this function: one plaintext multiply and one rotation. I don't have a change to try that would reduce the time.

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
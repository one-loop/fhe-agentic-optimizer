I have no new circuit to propose. The baseline (`(input * mask) << offset`) is the cheapest valid form I can find, and each alternative either adds operations or is rejected by the backend. I'm leaving it as the best kernel and not spending the remaining attempts on variants I expect to be slower.

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
I have no cheaper circuit for the last attempt either. The baseline (`(input * mask) << offset`) stays the best kernel. It uses one plaintext multiply with rescale and one rotation, and I can't remove either without changing the result or using an operation the backend rejects.

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
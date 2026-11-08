Idea: rotate the ciphertext first and rotate the plaintext mask by the same offset, then do the plaintext multiply. This is the same function, since rotation commutes with slot-wise multiplication. The mask rotation is a plaintext-only operation, so it should be cheap, and the circuit changes in ordering and operation levels. It may be rejected by the DSL, but it is worth testing.

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
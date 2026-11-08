The idea is to move the rotation off the ciphertext. Since `(input * mask) << k` equals `(input << k) * (mask << k)`, I can rotate the plaintext mask instead and multiply it with the rotated input. That could change the circuit, but it still needs a ciphertext rotation of the input. It only pays off if rotating the plaintext is cheaper. I'm not sure the DSL allows rotating a Plaintext, so this attempt tests that.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext rmask = mask << offset;
  Ciphertext output = (input << offset) * rmask;
  output.set_output("output");
}
```
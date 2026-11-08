This attempt tries a different structure: rotate the plaintext mask instead of the product, then do a single multiply. It rotates the input at the top level, where rotation is slightly more expensive. It only helps if the plaintext rotation is allowed and cheap. It is a gamble, but it is the only structurally different circuit left.

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
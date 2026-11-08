Idea: rotate the ciphertext first and rotate the plaintext mask as well, so the multiply is the last operation. The plaintext rotation should be handled in encoding rather than as a homomorphic rotation. This might change the operation mix, but it may not be cheaper.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext rotated = input << offset;
  Plaintext pm = mask << offset;
  Ciphertext output = rotated * pm;
  output.set_output("output");
}
```
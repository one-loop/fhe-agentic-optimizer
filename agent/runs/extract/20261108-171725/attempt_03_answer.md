A cheaper circuit than one multiply plus one rotation does not seem possible, so this attempt only changes how the rotation is written. It uses a right rotation by `slots() - offset`, which moves slots by the same amount as the left rotation by `offset`. CHEHAB will probably normalize it back to the baseline circuit, in which case it will be rejected again.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) >> (static_cast<int>(slots()) - offset);
  output.set_output("output");
}
```
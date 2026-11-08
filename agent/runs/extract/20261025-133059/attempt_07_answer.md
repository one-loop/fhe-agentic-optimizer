The circuit can't be reduced further, so I'm submitting the same minimal form. It uses one plaintext multiply at the first level and one rotation on the already-reduced ciphertext. Any other correct arrangement I can construct has equal or higher cost.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  Ciphertext output = (input * mask) << static_cast<int>(slots() / 4);
  output.set_output("output");
}
```
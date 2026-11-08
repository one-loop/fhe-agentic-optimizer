The baseline already uses the minimum operation counts of one plaintext multiply and one rotation. The only structurally different circuit I can think of is to rotate the ciphertext first and multiply by a rotated mask, `(input << k) * (mask << k)`. The rotation then runs at the top level, which is likely slower. It also depends on the DSL allowing a rotation of a plaintext, so this attempt is exploratory.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Plaintext rm = mask << offset;
  Ciphertext output = (input << offset) * rm;
  output.set_output("output");
}
```
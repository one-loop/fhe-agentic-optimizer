The new best is 29.14 ms (attempt 4), but that is within the 29.14–29.64 ms noise band seen across identical operation counts, so it is not a real improvement. The kernel is unchanged in structure.

I'm keeping this same kernel as the final answer. A rotation (~20 ms) and a ct-pt multiply (~5–7 ms) are both required, and the rotation already runs at the reduced level after the multiply.

```cpp
// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```
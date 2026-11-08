I don't see a cheaper circuit. The baseline already uses the minimum: one plaintext multiply, because the mask is an arbitrary input vector, and one rotation to move the window. The rotation already runs after the rescale, at a lower level, which is the cheaper order.

I can't rotate the mask first, because rotation needs a ciphertext operand and the mask is a plaintext. Splitting the rotation into several steps would add key-switches, and adding alignments would add work. Any variant I can build either has the same circuit as the baseline or costs more.

I'm resubmitting the baseline structure. It should be treated as a no-gain attempt, not a new best.

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
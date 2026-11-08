The baseline is already at the minimum I can find: one plaintext multiply with its rescale, and one rotation applied after the multiply, at a lower level. Rotating first would need a rotated mask, which CKKS rule 2 forbids, as attempt 1 showed.

The one thing left to try is expressing the same left shift as a right rotation by `slots() - offset`. CHEHAB may normalize this back to the baseline, in which case the circuit stays identical and nothing is lost. If it keeps a different rotation step, the timing might differ.

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
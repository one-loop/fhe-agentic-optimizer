**Idea:** The previous attempt failed because `first_slot >> 1` is a plaintext-plaintext operation, which the backend does not support. This version keeps only ciphertext rotations and plaintext multiplies. It masks each reduced coordinate with `first_slot` and combines them with a Horner chain, `out = m0 + ((m1 + (m2 >> 1)) >> 1)`. This places coordinate i in slot i with two rotations instead of the baseline's two separate shifts. The reduction is unchanged, so results stay exact for any indicator vector.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> masked;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    masked.push_back(r * first_slot);
  }

  // out = m0 + (m1 >> 1) + (m2 >> 2), evaluated as Horner to share rotations.
  Ciphertext inner = masked[1] + (masked[2] >> 1);
  Ciphertext output = masked[0] + (inner >> 1);
  output.set_output("output");
}
```
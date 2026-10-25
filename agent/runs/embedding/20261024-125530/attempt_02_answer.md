The remaining savings are small, so I only change the final combine step. Nesting the shifts as `(r1*f + (r2*f) >> 1) >> 1` keeps the rotation count at two but applies the second shift to an already-combined ciphertext. This is the same function.

I could not find a way to share the per-column reductions. Without plaintext masks other than `first_slot`, the columns mix if they are reduced together. Treat this as a low-expectation attempt.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext inner = (coordinates[2] * first_slot) >> 1;
  inner += coordinates[1] * first_slot;
  Ciphertext output = coordinates[0] * first_slot;
  output += inner >> 1;
  output.set_output("output");
}
```
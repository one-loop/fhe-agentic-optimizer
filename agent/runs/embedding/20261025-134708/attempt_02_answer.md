The baseline's final combination uses 2 rotations and 3 plaintext masks. I'll keep the three folded reductions and change only how the masked results are combined. I mask each column, shift the pair (c2 at slot 0) once by 1, add it to c1's masked slot 0, then shift that sum by 1 again. This is a nested Horner-style combination. It might change the rotation structure slightly, but I don't expect a large gain.

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

  Ciphertext m2 = coordinates[2] * first_slot;
  Ciphertext m1 = coordinates[1] * first_slot;
  Ciphertext m0 = coordinates[0] * first_slot;
  Ciphertext t = m1 + (m2 >> 1);
  Ciphertext output = m0 + (t >> 1);
  output.set_output("output");
}
```
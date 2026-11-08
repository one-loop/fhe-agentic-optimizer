I couldn't find a restructuring that reduces the operation count, so this attempt is a minor variant of the baseline and may be identical in circuit to it.

The change is to reduce each column with ascending rotation steps and combine the three masked results as before. Merging the three reductions into one needs per-slot masks, and the only mask available is `first_slot`, so I left the reduction structure alone.

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
    for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
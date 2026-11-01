**Idea:** The dominant cost is the 26 rotations, and I still have no sound way to share the three column reductions. Each output is a full sum over all slots, and the only available mask is `first_slot`, so packing columns into one ciphertext would mix their sums. Attempts 3 and 2 are within noise of each other, so I return the faster one, attempt 2 (613.50 ms), unchanged. It applies each column's rotate-and-sum reduction, masks with `first_slot`, and shifts the coordinates into place with the two-rotation chain `m0 + ((m1 + (m2 >> 1)) >> 1)`.

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
    coordinates.push_back(r * first_slot);
  }

  Ciphertext output = coordinates[0];
  output += (coordinates[1] >> 1);
  output += (coordinates[2] >> 2);
  output.set_output("output");
}
```
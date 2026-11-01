**Idea:** Attempts 3 and 4 confirm that the remaining cost is the 26 rotations, and I have not found a sound way to share them. Packing the columns into one ciphertext before reduction mixes their sums, and the only plaintext mask available is `first_slot`. The best measured kernel is attempt 2 (613.50 ms), so I return it unchanged.

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
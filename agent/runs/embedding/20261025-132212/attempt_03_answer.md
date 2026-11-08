I could not find a circuit with fewer operations. The columns can only be separated by a mask after the reduction, and `first_slot` is the only mask available. This variant reduces in ascending shift order and masks before combining, so it is another reordering of the same work and is unlikely to beat the baseline.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> m;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    m.push_back(r * first_slot);
  }

  Ciphertext output = m[0] + ((m[1] + (m[2] >> 1)) >> 1);
  output.set_output("output");
}
```
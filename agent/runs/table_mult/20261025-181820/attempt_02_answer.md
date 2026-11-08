I could not find a valid way to cut rotations. Each output needs its own full reduction, and merging outputs would need mask plaintexts or constants, which CKKS rule 1 forbids. For this attempt I'll reduce in ascending step order (1, 2, 4, …). It may be normalized to the same circuit as before, but it is the only change I can make without breaking the rules.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  vector<Ciphertext> rs;
  for (size_t i = 0; i < columns.size(); ++i)
    rs.push_back(indicator * columns[i]);
  for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
    for (size_t i = 0; i < rs.size(); ++i)
      rs[i] += rs[i] << static_cast<int>(steps);
  for (size_t i = 0; i < rs.size(); ++i)
    rs[i].set_output("output_" + to_string(i));
}
```
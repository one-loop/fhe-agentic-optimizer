I could not find a legal way to cut rotations, so this attempt is not expected to win.

The three sums are independent scalar reductions. Packing them into one ciphertext would need 0/1 slot masks, and CKKS rule 1 forbids building constants. The tables are arbitrary data, so they can't serve as masks either. Rotating the indicator once and sharing it across columns would require rotating the plaintext tables, which is plaintext arithmetic and also not allowed. Each reduction therefore still needs log2(slots) rotations.

This attempt changes the evaluation order to a tree-style layout. It still computes the same function, and it will probably produce the same operation counts.

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
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
    for (size_t i = 0; i < rs.size(); ++i)
    {
      Ciphertext t = rs[i] << static_cast<int>(steps);
      rs[i] = rs[i] + t;
    }
  for (size_t i = 0; i < rs.size(); ++i)
    rs[i].set_output("output_" + to_string(i));
}
```
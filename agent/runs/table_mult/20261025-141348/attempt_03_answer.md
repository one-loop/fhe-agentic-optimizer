I haven't found a circuit with fewer rotations. Packing the three products into one ciphertext needs plaintext masks, and the only plaintexts available are the table inputs. Separating the three sums from a mixed ciphertext is therefore impossible, and three independent full reductions are needed. The baseline's 24 rotations look like the minimum under these rules, so I'm resubmitting it unchanged, with the rotation order the same as the baseline.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
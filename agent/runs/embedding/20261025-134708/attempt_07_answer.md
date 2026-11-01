I couldn't find a cheaper structure within the CKKS and DSL constraints, so I'm returning the baseline unchanged.

CHEHAB already normalizes the reorderings I tried to the same circuit: 26 rotations, 6 plaintext multiplications, depth 2. The one structurally different version (folding to period 2 and finishing the odd and even slot classes separately) added plaintext multiplications and rotations and measured slower (677 ms vs 619 ms).

Combining the three columns into a single reduction fails because the per-column sums mix. Splitting them again needs masks, and the only mask available is `first_slot`.

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

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
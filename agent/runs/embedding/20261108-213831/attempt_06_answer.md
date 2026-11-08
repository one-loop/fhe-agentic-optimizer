I have no cheaper circuit to propose. After five attempts, I'm confident the baseline's operation counts are the minimum this task allows. That is 26 rotations, 6 ct-pt multiplies (3 tables and 3 masks) and 2 levels.

This round I also tried to make one rotation do two jobs. The idea was to feed an already-masked value into the last reduction step of another column. It fails because the column's sum is the same in every slot and must be masked afterwards. That later mask zeroes the shifted copy the shared rotation produced, so the rotation can't be shared.

The baseline below is my final answer. Further attempts can only produce rearrangements that CHEHAB normalizes back to this circuit, or deeper variants that are no faster.

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
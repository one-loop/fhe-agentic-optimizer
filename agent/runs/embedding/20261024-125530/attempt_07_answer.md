I have no further verified improvement, so this is again the best-measured kernel (1276 ms in attempt 4). Its operation counts match the baseline's.

I could not find a way to cut the rotations or multiplications. Any combination of the three products before the fold is shift-invariant, so it gives only c0+c1+c2 and cannot separate the columns. Only `first_slot` is available as a mask, so each column needs its own full fold and its own mask.

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

  Ciphertext inner = (coordinates[2] * first_slot) >> 1;
  inner += coordinates[1] * first_slot;
  Ciphertext output = coordinates[0] * first_slot;
  output += inner >> 1;
  output.set_output("output");
}
```
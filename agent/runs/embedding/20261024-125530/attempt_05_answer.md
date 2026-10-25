I have no new verified change, so I'm resubmitting the best-performing kernel (1276 ms). The gain over the baseline looks like timing variance, since the operation counts are identical.

The reductions can't be merged without plaintext masks, and `first_slot` is the only one available. The three masks, the two shifts and the 26 rotations are the minimum I can reach.

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
Across the attempts so far, the best measured time was 1276 ms, against 1313 ms for the baseline. The operation counts are identical in every run (26 rotations, 6 plaintext multiplies), so I treat that difference as run-to-run noise. I found no structural improvement.

For the final attempt I am resubmitting the same kernel. The blocker is that the three full-sum reductions can't be merged: any combination of the products before the fold is shift-invariant and gives only c0+c1+c2. `first_slot` is the only mask available, so each column needs its own fold and its own mask.

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
No new kernel this time. After six attempts I'm confident no cheaper circuit exists for this task, so I'm resubmitting the baseline as my final answer.

The baseline already uses the fewest operations of each kind this task allows:
- **26 rotations:** 8 per column to sum all 256 slots, plus 2 to move the second and third sums into slots 1 and 2.
- **6 plaintext multiplies:** one per table column, and one `first_slot` mask per output value to zero the other slots.
- **2 levels:** one for the table multiply and one for the mask.

The two alternatives I tried in the last two rounds don't work:
- **Trading a rotation for an extra mask:** a mask is cheaper than a rotation, but moving a value into slot 0 still needs the rotation.
- **Doing some reduction rotations after the mask, where they would be cheaper:** the mask leaves only slot 0, so the reduction must finish before it.

Every remaining rewrite either normalizes back to this circuit or, like attempt 1, is deeper and no faster.

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
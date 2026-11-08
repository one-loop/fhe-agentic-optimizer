I still have no cheaper circuit, so for the last attempt I'm submitting the baseline again; it will not beat the best.

Across all eight attempts, every rewrite either normalized back to the baseline circuit or, like attempt 1, came out deeper and no faster. The baseline already uses the fewest operations this task allows:

- **26 rotations:** 8 per column to sum all 256 slots, plus 2 to move the second and third sums into slots 1 and 2.
- **6 plaintext multiplies:** 3 by the table columns and 3 `first_slot` masks.
- **2 levels:** one for the table multiply and one for the mask.

The masks are needed because rotations and additions alone cannot zero the other slots. Columns cannot share rotations because the only mask available is `first_slot`, which isolates a single slot.

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
This is a last probe with a different operation order: fold the stride-128 step across all three products in one place, then run the remaining folds per column, mask, and combine with nested shifts. I have not found a way to cut the per-column rotations. With 256 slots, each column needs 8 fold rotations, and I could not separate the columns without extra masks.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext acc;
  for (int i = 2; i >= 0; --i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    Ciphertext m = r * first_slot;
    if (i == 2)
      acc = m;
    else
    {
      acc = acc >> 1;
      acc += m;
    }
  }
  acc.set_output("output");
}
```
I can't find a legal circuit cheaper than the baseline, so I'm submitting it again unchanged rather than a worse variant.

The counts are already at the floor for this DSL:
- **Rotations:** 8 per column, which is log2(slots) for rotate-and-add.
- **Multiplications:** 3 plaintext multiplications, one per column.
- **Packing:** sharing rotations across columns needs 0/1 masks, which the rules forbid.
- **Algebraic rewrites:** subtraction-based versions need as many reductions and add extra operations. One of them also failed with a transparent ciphertext.

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
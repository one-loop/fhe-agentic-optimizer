I have no cheaper valid circuit, so I'm submitting the baseline kernel again unchanged. Each output needs its own reduction of `indicator * table_i`, and the DSL gives no way to share rotations between columns without rotated plaintexts or masks. My other ideas, such as packing columns or deriving one output as a difference, either cannot be separated afterwards or add a fourth reduction.

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
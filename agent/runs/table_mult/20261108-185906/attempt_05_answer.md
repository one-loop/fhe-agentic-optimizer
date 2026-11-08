I haven't found a cheaper circuit, and I don't think one exists within the DSL rules. Each output needs its own full 8-rotation reduction of `indicator * table_i`. Sharing across columns would need rotated plaintexts, which are rejected, or masks, which can't be built. Deriving one output as `total - S0 - S1` adds a fourth reduction, so it is slower. I'm leaving the baseline circuit as the best kernel.

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
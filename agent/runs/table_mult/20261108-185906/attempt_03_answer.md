I can't find a cheaper valid circuit here. Rotating plaintext tables is rejected (plaintext-plaintext operations aren't supported). Sharing rotations across columns would need masks, and the DSL has no way to build them from constants. I'm resubmitting the baseline structure, which is already minimal at 8 rotations per column and one multiply level.

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
The baseline already uses the minimum rotate-and-add reduction for each column (8 rotations each). I found no sound way to share rotations across columns. That would need masks or plaintext rotations, and the DSL allows neither, since constants and plaintext-plaintext arithmetic are not permitted. This attempt reduces in ascending step order and keeps the multiplication count, so it is mainly a probe of whether CHEHAB's schedule changes the circuit. I don't expect a significant speedup from it.

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
    for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
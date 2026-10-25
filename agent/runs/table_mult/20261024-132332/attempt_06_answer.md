This attempt's gain (1175 ms) comes from run-to-run noise, since the operation counts are identical in every run. My earlier analysis still holds: with no plaintext masks and no plaintext rotation available, the 24 rotations and 3 plaintext multiplies are the minimum. I'm resubmitting the same kernel.

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
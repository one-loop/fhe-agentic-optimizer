I don't see a cheaper circuit. Plaintext rotation is rejected, which kills the shared-rotation idea. Without masks, the three products can't be packed into one ciphertext, so each of the three reductions needs its own 8 rotations. Any restructuring I can think of either keeps the same operation counts or adds operations. I'm resubmitting the baseline form, which cannot win but is correct.

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
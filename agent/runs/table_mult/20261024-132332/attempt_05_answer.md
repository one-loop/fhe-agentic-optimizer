The last run (1198 ms) was the same baseline structure as the others, so the spread across runs (1198–1507 ms) is measurement noise. I found no structural change that reduces work:

- **Rotations:** each column already uses the minimum log2(slots) rotations, and they run after the multiply at a lower level.
- **Packing columns:** this needs plaintext masks, which the DSL does not allow.
- **Sharing indicator rotations:** this needs plaintext rotation, which the backend rejected in attempt 3.

I'm resubmitting the same kernel.

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
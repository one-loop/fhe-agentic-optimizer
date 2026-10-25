I found no kernel that does less work than the baseline structure, so this is the same kernel again. Attempt 5 (1175 ms) is the best measured time, but it ran identical operations, so the 1175–1231 ms spread is noise rather than an improvement.

Cutting the 24 rotations would need one of two things the DSL doesn't offer:
- plaintext masks, to pack the three columns into one ciphertext, or
- plaintext rotation, to share indicator rotations across columns. The backend rejected this in attempt 3.

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
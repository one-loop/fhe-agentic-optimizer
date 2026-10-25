This is the last attempt. I found no change that reduces the work, so I'm submitting the same kernel. The 1175–1231 ms spread across runs is measurement noise, because every run executes identical operations (24 rotations, 3 plaintext multiplies, 24 adds).

Reducing the 24 rotations would need plaintext masks to pack the columns or plaintext rotation to share indicator rotations. The DSL has no constants, and the backend rejected plaintext rotation in attempt 3.

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
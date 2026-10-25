Plaintext rotation isn't supported by the backend, so the shared-rotation idea fails, and I'm back to the baseline structure.

Three independent total sums need either plaintext masks to pack the columns or plaintext rotations to share ciphertext rotations. The DSL provides neither. Each column already uses the minimum log2(slots) rotations, applied after the multiply at a lower level. The kernel below is the baseline again.

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
I found no way to cut rotations, so this attempt resubmits the baseline structure.

The three columns can only share rotations if their products are packed into one ciphertext. That needs plaintext masks, and the DSL forbids constants, so I can't build them. Each column's reduction already uses the minimum of log2(slots) = 8 rotations, which gives 24 in total. Each rotation also runs after the multiply, at a lower level, so it is already as cheap as it can be. The 1507 ms in attempt 1 against 1231 ms for the baseline came from running an identical operation count, so I read it as measurement noise.

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
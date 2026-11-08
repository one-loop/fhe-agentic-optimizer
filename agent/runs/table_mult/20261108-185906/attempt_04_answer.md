I have no valid circuit that beats the baseline, so I'm submitting the same minimal structure again. Each output depends only on its own product `indicator * table_i`, so it needs its own log2(slots) rotate-and-add chain (8 rotations per column). Sharing work across columns would need rotated plaintexts or masks, and the DSL doesn't allow either. Merging columns into one ciphertext also can't be undone without masks. The multiply is already done before the rotations, so they run at the cheaper post-rescale level.

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
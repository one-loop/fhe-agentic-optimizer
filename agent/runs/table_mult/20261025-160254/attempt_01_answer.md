Each output is a full slot sum of indicator*table_i, which costs 3 plaintext multiplies and 3×log2(16384) = 3×14 = 42 rotations in the naive form. The baseline reports 24 rotations, so CHEHAB seems to have already trimmed some work.

The idea is to share the rotation reduction across the three columns. Rotation is linear, so I can pack the three products into one ciphertext before reducing. This needs masking, and masks cost plaintext multiplies, but the masks would have to come from plaintext inputs, which I don't have. Constants can't be created, so masking is not possible.

Instead I use a different split. The full reduction of the first 3 levels over different columns can't be merged without masks, so I keep each column separate. What I can do is cut the cost of the final steps. The baseline already uses 8 rotations per column, which suggests the rewrite pass reduced it. I'm not confident I can beat that by a lot.

An option that reduces work is to sum the products first and rotate once. Because the tables are plaintexts, I can't combine them with a mask, so this doesn't work either.

So I keep the structure and make one small change: multiply by the plaintext after part of the reduction. That would cost more multiplies, so it doesn't help. I resubmit the baseline form, restructured so each column's reduction runs in a different order (large steps last), which may change the circuit slightly.

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
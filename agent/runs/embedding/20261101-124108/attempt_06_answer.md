Attempt 5 confirms that depth dominates: one level shallower cost about 39 ms. So depth 10, with products at level 9 and masks at level 10, stays. This attempt changes only how the zero term is formed: `w = y8 - y1`. Since `first_slot` is idempotent, `y1 = y8 = indicator[0]·e0`. The partner of `y8` is now `y1`, which is otherwise dead after the chain, so CHEHAB's alignment work moves to different operands. The rest is identical to the best kernel.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  // y_k = indicator * first_slot^k all equal indicator[0]*e0 for k >= 1.
  Ciphertext y1 = indicator * first_slot;
  Ciphertext y = y1;
  for (int k = 0; k < 7; ++k)
    y = y * first_slot;
  Ciphertext y8 = y;
  Ciphertext w = y8 - y1;          // numerically zero, level 8
  Ciphertext ind = indicator + w;  // indicator, dragged to level 8

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = ind * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
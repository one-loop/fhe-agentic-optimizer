The deep term no longer needs a rotation. Every `y_k = indicator * first_slot^k` (k ≥ 1) has the same value, `indicator[0]` in slot 0, so `w = y8 - y7` is zero up to noise, and `y7` comes free as an intermediate of the chain.

I add `w` to `indicator` once, before the table products. Only one ciphertext then needs dragging down to level 8. All three products, the 24 reduction rotations, the masks and the placement rotations then run at the deepest levels. That removes the extra rotation, most of the mod switches and the 3 scale matches, and the 3 table products become cheap deep multiplies.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  // y_k = indicator * first_slot^k all equal indicator[0]*e0 for k >= 1.
  Ciphertext y = indicator * first_slot;
  for (int k = 0; k < 6; ++k)
    y = y * first_slot;
  Ciphertext y7 = y;
  Ciphertext y8 = y7 * first_slot;
  Ciphertext w = y8 - y7;          // numerically zero, level 8
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
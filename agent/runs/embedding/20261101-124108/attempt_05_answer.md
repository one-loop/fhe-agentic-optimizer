The structure seems close to optimal: 26 rotations and 3 masks, all at the deepest levels. The remaining overhead is the 8-step `first_slot` chain, whose early multiplies run at expensive top levels. This attempt measures the trade-off directly: the chain is one step shorter (7 multiplies), the products move to level 8, the reduction runs at level 8, and the masks at level 9. That saves one chain multiply plus some alignment work, at the price of slightly costlier rotations. I estimate it is more likely slower than faster, so this is mainly a measurement.

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
  for (int k = 0; k < 5; ++k)
    y = y * first_slot;
  Ciphertext y6 = y;
  Ciphertext y7 = y6 * first_slot;
  Ciphertext w = y7 - y6;          // numerically zero, level 7
  Ciphertext ind = indicator + w;  // indicator, dragged to level 7

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
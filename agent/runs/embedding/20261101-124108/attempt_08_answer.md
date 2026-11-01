The circuit looks close to its floor: 26 rotations, minimal masks, and maximum depth with everything at the deepest levels. The remaining variable is where CHEHAB places its alignments. Here I form the zero term one level earlier, as `y7 - y6`, and push it to level 8 with one more `first_slot` multiply. The scale match between the two chain branches then happens at level 7 rather than 8, and `indicator` aligns against a single chain output. The multiply count is unchanged (8 chain, 3 table, 3 mask), and the products, reductions and masks all stay at levels 9–10.

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
  Ciphertext w = (y7 - y6) * first_slot;  // numerically zero, level 8
  Ciphertext ind = indicator + w;         // indicator at level 8

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
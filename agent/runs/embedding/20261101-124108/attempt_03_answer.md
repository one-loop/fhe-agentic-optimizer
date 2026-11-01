The remaining overhead is the two scale matches and seven mod switches spent aligning `y7` and `indicator` around the zero term. I regroup the sum as `(indicator - y7) + y8`. `indicator` is then aligned to `y7`'s level, and the result meets `y8` one level deeper. This aims to merge the alignments into a single scale match plus mod switches. The values are unchanged because `y7 = y8` numerically, and everything downstream stays at the deepest levels.

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
  Ciphertext t = indicator - y7;   // indicator - ind0*e0, aligned to level 7
  Ciphertext ind = t + y8;         // == indicator, at level 8

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
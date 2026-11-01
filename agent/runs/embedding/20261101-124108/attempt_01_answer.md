(y << 1)`. Its slot sum is exactly 0, so it does not change any reduction result.
3. Add `w` to each `r_i` before its reduction. CHEHAB then mod-switches `r_i` down to level 9 (~1 ms). All 24 reduction rotations, the 3 masks and the 2 placement rotations then run at the deepest levels, where they cost far less.
4. The overhead is about 8 cheap plaintext multiplies plus one deep rotation, which should cost much less than what is saved on the 26 rotations.

Total multiplicative depth is 10.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> prods;
  for (const auto &column : columns)
    prods.push_back(indicator * column);

  // Deep zero-sum term: y at level 9, w = y - rot(y) has total sum 0.
  Ciphertext y = prods[0] * first_slot;
  for (int k = 0; k < 7; ++k)
    y = y * first_slot;
  Ciphertext w = y - (y << 1);

  vector<Ciphertext> coordinates;
  for (auto &p : prods)
  {
    Ciphertext r = p + w;
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
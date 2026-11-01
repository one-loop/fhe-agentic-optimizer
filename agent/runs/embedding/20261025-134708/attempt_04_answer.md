This attempt folds to period 2 (steps 8192 … 2, so 13 rotations per column) and finishes each column with one more rotation and add. It then masks and places each total. The circuit is different, but I'm not sure it will be faster.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> w;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 1; steps >>= 1)
      r += r << static_cast<int>(steps);
    w.push_back(r);
  }

  // slot 0 of w_i holds the even-class sum, slot 1 the odd-class sum.
  // Mask slot 0 of each, place at slot i; do the same for the odd class
  // after a single shared combination.
  Ciphertext even = w[0] * first_slot;
  Ciphertext odd = (w[0] << 1) * first_slot;
  for (size_t i = 1; i < w.size(); ++i)
  {
    even += (w[i] * first_slot) >> static_cast<int>(i);
    odd += ((w[i] << 1) * first_slot) >> static_cast<int>(i);
  }
  Ciphertext output = even + odd;
  output.set_output("output");
}
```
The zero term now comes from a different identity. `z = indicator - indicator*first_slot` has a 0 in slot 0, so `z*first_slot` is exactly zero, and multiplying it by `first_slot` six more times keeps it zero while reaching level 8. Adding that to `indicator` drags `indicator` to level 8 as before. This drops the separate `y7`/`y8` branch: the chain runs on `z` itself. The products, reductions, masks and placement stay at the deepest levels, as in the best kernel.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext y1 = indicator * first_slot;  // indicator[0]*e0
  Ciphertext z = indicator - y1;           // slot 0 is zero
  Ciphertext w = z * first_slot;           // numerically zero
  for (int k = 0; k < 6; ++k)
    w = w * first_slot;                    // still zero, level 8
  Ciphertext ind = indicator + w;          // indicator at level 8

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
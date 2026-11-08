I could not find a cheaper circuit within the tools available. The mask and shift structure needs 3 masks, and with only `first_slot` as a mask the three reductions cannot share rotations. Packing columns into disjoint slots would need other masks.

This attempt only reverses the reduction stride order and applies the mask Horner-style. It is correct, but CHEHAB will probably normalize it to the same circuit.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> m;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    m.push_back(r * first_slot);
  }
  Ciphertext y = (m[2] >> 1) + m[1];
  Ciphertext output = (y >> 1) + m[0];
  output.set_output("output");
}
```
Merging the three reductions would need per-slot masks, and only `first_slot` is available, so I can't cut the rotation count. This attempt only changes how the three masked results are combined, building them as a chain instead of independent shifts. It is likely to land on the same circuit, but it is the only structural change I can offer now.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> c;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    c.push_back(r);
  }

  Ciphertext b = (c[2] * first_slot) >> 1;
  b += c[1] * first_slot;
  Ciphertext output = (b >> 1) + c[0] * first_slot;
  output.set_output("output");
}
```
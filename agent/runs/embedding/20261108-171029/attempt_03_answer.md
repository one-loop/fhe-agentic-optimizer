I still can't cut rotations, since packing the three columns before reduction needs masks I don't have. This attempt only reorders the shifts: the shared right-shift is applied to a partial sum instead of to each masked term. It is likely to give the same circuit.

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

  Ciphertext a = c[0] * first_slot;
  Ciphertext b = c[1] * first_slot;
  Ciphertext d = c[2] * first_slot;
  Ciphertext output = a + ((b + (d >> 1)) >> 1);
  output.set_output("output");
}
```
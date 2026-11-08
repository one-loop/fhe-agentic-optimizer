I don't have a structurally cheaper circuit. Every packing scheme I tried needs slot masks other than `first_slot`, or loses the separation between the three columns. As a last variation, this kernel builds the output by chaining right-shifts and adds the unmasked `c[0]` term before masking. It may still land on the same circuit as before.

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

  Ciphertext d = c[2] * first_slot;
  Ciphertext e = (d >> 1) + c[1] * first_slot;
  Ciphertext output = c[0] * first_slot + (e >> 1);
  output.set_output("output");
}
```
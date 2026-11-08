This is my last attempt, and I don't have a cheaper circuit. This kernel is attempt 2 again, which was the fastest single timing (130.81 ms) but did not confirm against the baseline. It needs exactly two shifts to place the masked values in slots 1 and 2, and I see no way to lower the three reductions or the three plaintext masks.

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
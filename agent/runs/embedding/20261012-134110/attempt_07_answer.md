I could not find a kernel that beats attempt 2 (613.50 ms, against a 620.08 ms baseline). The remaining cost is the 26 rotations, and I have no sound way to share them across the three columns with the available operations. Attempts 2 to 6 all land in the 613 to 621 ms range, so the differences are noise. I am resubmitting attempt 2.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> masked;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    masked.push_back(r * first_slot);
  }

  // out = m0 + (m1 >> 1) + (m2 >> 2), evaluated as Horner to share rotations.
  Ciphertext inner = masked[1] + (masked[2] >> 1);
  Ciphertext output = masked[0] + (inner >> 1);
  output.set_output("output");
}
```
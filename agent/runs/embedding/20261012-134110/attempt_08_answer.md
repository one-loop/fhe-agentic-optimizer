I'm resubmitting the attempt 2 kernel, the fastest measured (613.50 ms against the 620.08 ms baseline). The remaining cost is the 26 rotations, and the other attempts since then have landed within noise of it. I have no sound way to share the rotations across the three columns with the operations available here.

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
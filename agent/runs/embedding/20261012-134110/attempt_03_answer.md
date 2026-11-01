**Idea:** I could not find a sound way to cut the rotation count below the current 26. Each column's total must come from its own full rotate-and-sum, and I have no plaintext masks other than `first_slot`. Plaintext-plaintext operations are rejected by the backend, so I cannot build masks from the tables. Packing columns into one ciphertext would mix their sums, so I keep the best kernel so far (attempt 2, 613.50 ms) unchanged.

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
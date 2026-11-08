This is a modest restructuring and I expect it to be at best marginal. I mask S1 and S2 first and shift the combined pair once, which changes the order of operations but not the number of reductions. I did not find a way to share one reduction across the three columns. Because the reduction fills every slot, the columns can only be separated by masks applied after it.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> s;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    s.push_back(r);
  }

  Ciphertext m1 = s[1] * first_slot;
  Ciphertext m2 = s[2] * first_slot;
  Ciphertext tail = m1 + (m2 >> 1);
  Ciphertext output = s[0] * first_slot + (tail >> 1);
  output.set_output("output");
}
```
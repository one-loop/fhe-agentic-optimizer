I have no cheaper circuit for the last attempt either. Each of the three outputs needs its own full reduction of 8 rotations, and I found no way to pack, mask or share rotations across columns with the available operations. I'm submitting the correct baseline structure as my final answer. It cannot beat the best, and I have no faster kernel to offer.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
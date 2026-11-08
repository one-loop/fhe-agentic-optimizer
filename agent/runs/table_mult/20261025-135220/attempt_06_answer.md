I have no legal improvement left, so this is the baseline circuit again.

Every route I examined, including the packing and subtraction variants, either needs slot masks or keeps the same number of reductions. Each column's sum depends on every slot of its own product, so each needs its own 8 rotate-and-add steps. I'd rather keep the best-known correct circuit than risk a slower or failing one.

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
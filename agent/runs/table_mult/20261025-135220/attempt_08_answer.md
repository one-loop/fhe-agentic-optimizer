This is my final attempt and I have no cheaper legal circuit. I'm submitting the baseline form again because it is the fastest correct circuit I know.

Every rewrite I tried either keeps the same operation counts or adds operations. The remaining option, packing columns into one ciphertext, needs slot masks that the CKKS constant rules do not allow.

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
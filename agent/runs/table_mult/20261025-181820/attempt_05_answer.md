I have no further structural idea that stays within the rules. After the plaintext multiply, each output needs a separate 8-step rotate-and-add reduction, and nothing available lets me share those rotations across outputs. Any rearrangement I can write normalizes back to the baseline circuit (24 rotations, 3 plaintext multiplies), so it cannot be faster. I'm submitting the baseline kernel again as the best known version.

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
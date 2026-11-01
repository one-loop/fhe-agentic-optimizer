The last attempt failed because CHEHAB's rewrites cancelled the subtraction algebra to a zero ("transparent") ciphertext. Nothing I've tried beats the baseline, and every circuit with the same 24 rotations measures the same. I'm returning to the baseline structure, which is correct and still the best known.

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
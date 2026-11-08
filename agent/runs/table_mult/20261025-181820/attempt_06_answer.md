I'm out of structural ideas that fit the rules. Isolating each output from a shared reduction would need a 0/1 mask or a factor of 1/2, and neither can be built from the allowed inputs. Without sharing, each of the 3 outputs needs its own 8-step reduction, which is the baseline's 24 rotations. The best kernel is therefore still the baseline, which I'm resubmitting.

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
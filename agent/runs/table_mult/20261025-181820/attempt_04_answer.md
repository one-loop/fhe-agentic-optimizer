I have not found a cheaper circuit, so I'm not going to present a rewrite as an improvement. A reduction only returns integer-weighted combinations of the three sums. Isolating one output from a shared reduction would need a mask or a factor of 1/2, and neither can be built from the allowed inputs. The three full reductions of 8 rotations each therefore seem necessary. I'm resubmitting the best known kernel unchanged.

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
I haven't found a cheaper circuit than the baseline, and I'm out of useful ideas. Each output needs its own 8-rotation reduction of `indicator * table_i` at one multiplication level. Sharing rotations across columns would need rotated plaintexts, which CHEHAB rejects, or masks, which can't be built from constants. Attempts 1, 3, 4, 5, 6 and 7 all compiled to the same circuit as the baseline, and attempt 2 failed to compile. The baseline at 61.01 ms stays the best result. I'm submitting the same kernel one last time.

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
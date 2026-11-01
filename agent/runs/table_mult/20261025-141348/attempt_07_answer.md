The idea is to share one rotation of the indicator across all three columns. The first fold step of each column, r + (r << 128), equals ind·t + (ind << 128)·(t << 128). Rotating the indicator once and reusing it saves 2 ciphertext rotations in total, at the price of 3 extra plaintext multiplies. This relies on rotating the plaintext tables (`<<` on a Plaintext), which may not be supported, so the attempt could fail to compile.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int h = static_cast<int>(slots() >> 1);
  Ciphertext ind2 = indicator << h;
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Plaintext t2 = columns[i] << h;
    Ciphertext r = indicator * columns[i];
    r += ind2 * t2;
    for (size_t steps = slots() >> 2; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
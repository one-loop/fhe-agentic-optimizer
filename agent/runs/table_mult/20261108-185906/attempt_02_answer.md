I'll try rotating the indicator once and sharing it across all three columns. The first fold of each column then needs a rotated table (`t << 128`) and an extra plaintext multiply, which costs less than a rotation. This depends on the DSL accepting rotation of a plaintext input, so it may fail to compile.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int half = static_cast<int>(slots() >> 1);
  Ciphertext ind_rot = indicator << half;
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Plaintext tr = columns[i] << half;
    Ciphertext r = indicator * columns[i] + ind_rot * tr;
    for (size_t steps = slots() >> 2; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
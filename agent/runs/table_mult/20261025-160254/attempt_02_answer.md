The three columns cannot share a rotation tree, because there are no masks to pack them into one ciphertext. Instead I fold the first reduction step into the multiplies. I rotate `indicator` once and share it, and I use rotated plaintext tables:

y_i = ind·t_i + rot(ind,128)·rot(t_i,128)

This replaces 3 ciphertext rotations with 1 shared rotation, for 22 rotations in total. The cost is 3 extra plaintext multiplies. I have not confirmed that the DSL supports `<<` on a `Plaintext`, so this attempt may fail to compile.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int half = static_cast<int>(slots() >> 1);
  Ciphertext ind_r = indicator << half;
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Plaintext tr = columns[i] << half;
    Ciphertext r = indicator * columns[i] + ind_r * tr;
    for (size_t steps = slots() >> 2; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
Attempt 2 had an identical operation count to the baseline, so its timing difference is noise. This attempt tries to share rotations across columns by rotating the indicator instead of the products.

The first reduction steps expand as follows:
- Folding with a rotation by `s` on `ind*t_i` equals `ind*t_i + (ind<<s)*(t_i<<s)`.
- So for the first two steps, the rotated copies of `indicator` (shifts 0, 64, 128, 192) are shared by all three columns.
- That costs 3 shared rotations instead of 6.
- The cost is extra plaintext multiplies, which are cheap, plus plaintext rotations, which I'm not sure the DSL allows.

The remaining reduction steps (32 down to 1) stay as they were.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int n = static_cast<int>(slots());
  int q = n / 4;
  vector<Ciphertext> rot;
  for (int k = 0; k < 4; ++k)
    rot.push_back(k == 0 ? indicator : (indicator << (k * q)));
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Ciphertext r = rot[0] * columns[i];
    for (int k = 1; k < 4; ++k)
      r += rot[k] * (columns[i] << (k * q));
    for (int steps = q >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
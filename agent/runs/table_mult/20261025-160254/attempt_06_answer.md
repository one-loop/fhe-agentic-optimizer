I have nothing new to try, so this is the baseline again. Three reasons it can't be beaten under the rules:

- Each of the three outputs needs its own 8-step rotation tree.
- Packing columns together would need masks, which CKKS cannot create.
- Rotated plaintext tables are rejected as plaintext-plaintext arithmetic.

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
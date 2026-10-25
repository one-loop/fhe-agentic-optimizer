I can't find a circuit that beats the baseline, so I'm stopping with the baseline structure.

Beating it would need fewer than 24 rotations or fewer plaintext multiplications. Every route I examined fails:
- **Packing outputs:** it needs plaintext masks, and the only plaintexts available are the unknown tables.
- **Shared rotations:** they need rotated plaintext tables, which CHEHAB rejects.
- **Mixing columns before the reduction:** the three sums come out entangled and can't be separated.
- **Extra multiplies to reach a deeper level:** nothing I can add changes the function without constants.

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
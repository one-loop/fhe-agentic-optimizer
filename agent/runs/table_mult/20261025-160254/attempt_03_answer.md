I could not find a cheaper circuit than the baseline, so I am resubmitting it unchanged.

- **Shared rotations:** the three columns can't share a rotation tree, because mixing them into one ciphertext needs masks, and CKKS gives me no constants to build them from.
- **Folding steps into the multiplies:** this needs rotated plaintext tables, and the backend rejects plaintext-plaintext operations, so `table_i << k` fails (attempt 2).
- **Combining columns:** any linear combination such as `r0 + r1 + r2` still needs one full reduction per independent output, so it saves no rotations.

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
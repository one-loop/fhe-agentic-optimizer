I still see no cheaper circuit than the baseline, so I am resubmitting it. This attempt will be flagged as identical again.

Each output is a different linear functional of `indicator`, and each must end up replicated in every slot. That needs a separate 8-step rotation tree per output. Sharing work across columns would need masks to pack and unpack them, and CKKS offers none. Rotating the plaintext tables instead, to trade rotations for multiplies, is rejected as plaintext-plaintext arithmetic.

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
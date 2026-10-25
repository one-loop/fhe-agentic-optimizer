I still have no cheaper circuit to offer. The cost is driven by 3 independent full reductions of 8 rotations each, and the three sums cannot be packed into fewer ciphertexts without masks.

I looked at four ways to cut rotations, and none works:
- **Rotating plaintext tables:** CHEHAB rejects it as plaintext-plaintext evaluation.
- **Parity packing:** reducing over all steps but the last and packing two columns into even/odd slots cannot be separated afterwards.
- **Reducing linear combinations:** this still needs three independent reductions to recover three unknowns.
- **Rotating the indicator:** this changes the function being computed.

I'm resubmitting the correct baseline structure, which cannot beat the best, rather than a variant that adds operations.

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
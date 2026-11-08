I have no new circuit to offer. Every variant I can build either keeps the baseline's operation counts or is more expensive, so I am resubmitting the baseline.

- **Packing columns:** packing two or three columns into one ciphertext before the rotation tree mixes their sums, and unmixing them needs masks, which CKKS does not provide.
- **Derived outputs:** deriving one output as `T − S0 − S1` from a combined sum `T` adds operations.
- **Plaintext rotations:** rotating the plaintext tables is rejected as plaintext-plaintext arithmetic.

The baseline's 3 plaintext multiplies and 3×8 rotations (24 in total) look minimal under these rules.

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
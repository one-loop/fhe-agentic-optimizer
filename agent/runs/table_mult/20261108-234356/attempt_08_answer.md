No cheaper circuit exists within the CKKS rules here, so for the last attempt I'm returning the baseline, which is the best correct kernel.

Over the seven attempts, every correct kernel either compiled to the baseline's exact operation counts or added work. Attempt 2 added 4 additions and ran at 61.27 ms against the baseline's 60.93 ms. Attempt 6, which tried to share rotations, was rejected because CHEHAB doesn't allow rotating a plaintext table.

The baseline is already minimal:
- **3 plaintext multiplications and 3 rescales.** Each output needs its own product with a different table column, so these can't be removed.
- **24 rotations.** Each of the three outputs is an independent sum over all 256 slots, which takes 8 rotations.
- **Why no sharing or packing works.** Sharing the indicator's rotations across columns would need rotated tables, and packing the three products into one ciphertext would need 0/1 masks. Both are blocked by the bans on constants and plaintext-only operations.

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
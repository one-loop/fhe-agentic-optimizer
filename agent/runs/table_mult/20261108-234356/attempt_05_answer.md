The baseline circuit is the minimum this task allows. Attempts 1, 3 and 4 all compiled to exactly the baseline's operation counts. Attempt 2, the only structurally different variant that stayed correct, was slower at 61.27 ms against 60.93 ms.

| Operation | Count | Why it can't go lower |
|---|---|---|
| Plaintext multiplications | 3 | Each output needs its own product with a different table column. |
| Rescales | 3 | One per multiplication, at the 1-level minimum. |
| Rotations | 24 | Each output is an independent full sum over 256 slots, which needs 8 rotations. |

Cutting rotations would require packing the three products into one ciphertext with masks, or sharing the indicator's rotations through rotated tables. Both need constants or plaintext-only operations, which the CKKS rules forbid.

Any further "different" kernel can only add operations, so it cannot pass the 3%-faster check. I recommend ending the search here and keeping the baseline as the final result:

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
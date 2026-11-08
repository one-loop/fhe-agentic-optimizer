The baseline is optimal for this task. Three attempts have now confirmed the operation counts I expected: 3 plaintext multiplications, 3 rescales, 24 rotations and 24 additions. Every equivalent rewrite either collapses back to that circuit or adds cost, as attempt 2 did at 61.27 ms.

What would actually lower the cost is ruled out by the CKKS rules:
- **Packing the three products into one ciphertext** would need a 0/1 mask plaintext. Masks can't be built because numeric constants and plaintext-plaintext operations are forbidden.
- **Sharing the indicator's rotations across columns** would need rotated tables, which is a plaintext-only operation.
- **Recovering separate outputs from combined sums** would need a ½ factor, which is a constant.

Without these, each output is an independent all-slot sum that needs 8 rotations. A 1-level circuit can't avoid one plaintext multiplication and one rescale per output.

I recommend stopping here and keeping the baseline as the final answer. For completeness, this is the optimal kernel:

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
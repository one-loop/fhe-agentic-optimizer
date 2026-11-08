Attempt 2 confirms the analysis. Moving one reduction onto a combined ciphertext only adds additions, because the rotation, multiplication and rescale counts are fixed by the problem.

| Operation | Count | Why it can't go lower |
|---|---|---|
| Plaintext multiplications | 3 | Each output needs its own slot-wise product with a different table column. |
| Rescales | 3 | One follows each of those products, and the products never merge. |
| Rotations | 24 | No mask constants are available to pack the products into one ciphertext. Without packing, each of the 3 independent slot sums needs log₂(256) = 8 rotations. |
| Additions | 24 | One per rotation in a doubling tree, which is the minimum. |

Sharing rotations through the indicator would need rotated plaintext tables, which the CKKS rules forbid. Separating combined sums would need constants such as ½, which are also forbidden.

So the baseline is already optimal. The remaining attempts can't produce a correct circuit that is cheaper and also different, so I'm returning the optimal kernel unchanged instead of spending them on variants that only add cost.

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
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
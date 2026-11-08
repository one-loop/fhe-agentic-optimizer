Attempt 6 failed because CHEHAB rejects rotating a plaintext (`columns[i] << half`) as a plaintext-only operation.

There is no ciphertext-side way to get the same effect. Rotating the indicator, multiplying by the table, and rotating back produces products at mismatched slots. In that ordering, each slot holds the indicator value times a table value from a different slot, and the reduction needs matching pairs. Getting matching pairs again requires rotating the product itself, which is exactly what the baseline does.

That leaves no lever for this task:
- Rotation sharing needs plaintext rotation, which is forbidden.
- Packing the products into one ciphertext needs masks, which need constants, which are forbidden.
- The 3 products and 3 rescales are mandatory.
- 8 rotations per independent full-slot sum is the minimum.

The baseline circuit is therefore optimal, and I'm returning it as the final kernel. I recommend not spending the last attempt.

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
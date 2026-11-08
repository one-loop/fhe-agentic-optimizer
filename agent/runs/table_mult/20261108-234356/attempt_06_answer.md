## Idea for attempt 6

I can share rotations of the indicator across the three columns by also rotating the tables to match, with `t << k` on a Plaintext. I'm not sure the DSL accepts this: the rule that every operation needs a ciphertext operand may reject rotating a plaintext. If it does, this attempt fails at compile time.

The construction, with n = 256 slots:
- Rotate the indicator once, `ir = indicator << 128`, and reuse it for every column.
- For each column, compute `Q_i = indicator * t_i + ir * (t_i << 128)`. Slot y holds the products at slots y and y+128, so `Q_i` already has period 128.
- Only 7 folding rotations per output are then needed, instead of 8.

The cost changes are:

| | Baseline | Attempt 6 |
|---|---|---|
| Rotations | 24 | 1 + 3·7 = 22 |
| Plaintext multiplications | 3 | 6 |

At this 1-level chain, a ciphertext × plaintext multiply should cost less than a rotation, so this could be a net win.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int n = slots();
  int half = n >> 1;
  Ciphertext ir = indicator << half;
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Plaintext tr = columns[i] << half;
    Ciphertext r = indicator * columns[i] + ir * tr;
    for (int steps = half >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
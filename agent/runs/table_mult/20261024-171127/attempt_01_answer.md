Each output is a full sum over all slots of indicator * table_i, so the baseline needs 3 reductions of 8 rotations each, 24 rotations in total.

Idea: pack the three products into one ciphertext before the reduction. Plaintext inputs can't be masked with constants, so I can't isolate slot blocks. Combining by rotation doesn't work either, because a rotation of a total-sum ciphertext is the same ciphertext.

Reduction is linear, so I can reduce the first rotation levels jointly. Take A_i = indicator * table_i for i = 0, 1, 2 and do the first step with a different structure: after a rotate-and-add by the largest step, each A_i still needs its own further steps, so the saving has to come from elsewhere.

One alternative is a transpose-style reduction. It needs masks, which I can only get from plaintext inputs, so I can't build them.

So I can't cut the total rotation count below 24 without masks. I can still trim the work: the full sum is over 16384 slots and the baseline does 14 steps, but the printed count says rotate=24, which means slots() is 256. That gives 8 steps per column, so 24 in total.

The one safe optimization I can make is to rotate indicator once per step, then multiply by the plaintexts afterwards:

sum_i(ind * t_i) = sum over rotations of the product, which still needs the table rotations. Rotating plaintexts is free on the plaintext side only if I can compute them, but plaintext rotation isn't a ciphertext op, so it's not allowed.

I'll keep the structure, but move the multiply to the end using a rotated-indicator sum. This only trades costs, since it takes 255 rotations. It is worse, so I'll stay with the baseline form.

A real change is to share work across columns by reordering the adds. The rotate count stays the same, but I can use a tree with the same count and a different circuit that finishes at deeper levels. That brings little gain, and I have no better option, so I'm submitting a variant that interleaves the three columns.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
    r.push_back(indicator * columns[i]);
  for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
    for (int i = 0; i < 3; ++i)
      r[i] += r[i] << static_cast<int>(steps);
  for (int i = 0; i < 3; ++i)
    r[i].set_output("output_" + to_string(i));
}
```
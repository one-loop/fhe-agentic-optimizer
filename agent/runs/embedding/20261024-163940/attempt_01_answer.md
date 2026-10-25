Idea: the baseline does three full rotate-and-add reductions (3 × 14 = 42 rotations, shown as 26 after CHEHAB's simplification). Instead, I mask each coordinate first and share a single reduction.

Compute p_i = indicator * table_i for i = 0, 1, 2. Each coordinate's total must land in slot i, and every other slot must be 0. A rotate-and-add reduction leaves the sum in every slot, so the three coordinates can't simply be combined before it.

An alternative is to fold the placement into the tables. Multiplying by a plaintext mask costs a level and ~7 ms, which is much cheaper than a rotation, but the masking can't happen before the reduction without breaking the sums. So I combine the columns after the reduction:

1. Form r_i = reduce(p_i). That is 3 reductions, which is the baseline.
2. Instead, combine first: let q = p_0 + (p_1 >> 1) + (p_2 >> 2). Slot j of q holds p_0[j] + p_1[j+1] + p_2[j+2], which mixes the columns. The full sum of q then equals the sum of all three coordinates, which is not separable, so this doesn't work directly.

A better approach is a reduction that stops partway. Reduce each p_i within blocks, then combine:

- Do the first 11 reduction steps (strides 8192 down to 8) on a combined vector. The remaining steps would need to sum within blocks of 8 slots.
- Slots are cyclic, so the sum over all slots requires strides 8192 … 1 on every vector.
- Pack the three columns into one vector by masking with plaintexts. I only have first_slot as a mask, and I cannot build other masks since plaintext-plaintext arithmetic is not allowed, so I cannot build position-specific masks.

Using only first_slot, I do the following:

- Reduce the three products down to a partial sum with large strides, so the sum sits in 8 slots per vector, and combine them with shifts. Without other masks this is hard to keep separable.

The practical gain is to merge the masking: output = Σ (r_i * first_slot) >> i. Since r_i is a full-sum vector, reduction is linear. Compute s = p_0 * first_slot-free pieces... The masking can be moved before the final reduction steps only if the data stays separable, which it doesn't.

So I take a safe, smaller improvement: apply the reduction to the three vectors, but combine their shifts so that the three masks are replaced by one. Form t = r_0 + (r_1 >> 1) + (r_2 >> 2) with every slot holding a total, then multiply by first_slot once. That leaves only slot 0 with r_0 + r_1[-1] + r_2[-2], which is wrong, since all slots hold the same value per vector.

Instead: m_i = r_i * first_slot, which is the baseline's 3 plaintext multiplies. Alternatively, reduce only 2 vectors and one with a combined mask. This yields no clear win without more masks, so I keep the structure and cut the cost of the masks:

- Output = (r_0 * first_slot) + ((r_1 * first_slot) >> 1) + ((r_2 * first_slot) >> 2).
- Equivalent: ((r_1 + (r_2 >> 1)) * first_slot) >> 1 = (r_1 * fs) >> 1 + (r_2[-1] ...) which is wrong because r_2 is uniform and needs slot 2.
- Take u = r_0 + (r_1 >> 1) + (r_2 >> 2). With uniform r_i, slot 0 = r_0, slot 1 = r_1 + r_0, and so on. This is not clean.

So I submit the baseline with a modest restructure: shift-combine after masking, using fewer rotations (2 instead of the baseline's 2, so no change). I could not find a verified improvement, so this kernel is the baseline's computation in a slightly different form.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext output;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    Ciphertext m = r * first_slot;
    if (i > 0)
      m = m >> i;
    if (i == 0)
      output = m;
    else
      output += m;
  }
  output.set_output("output");
}
```
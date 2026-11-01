The baseline does three full rotate-and-add reductions (3×13 = 39 rotations, reduced to 26 by CSE?) plus masking. The cheaper plan is to combine the three columns before reducing, so only one reduction is needed.

Idea: first compute the three products p_i = indicator * table_i. Each must be summed across all slots, and the result for column i has to land in slot i.

Pack the products so a single reduction covers all three:
- Mask each p_i to slot 0 only after reduction, which is expensive. Instead, fold the placement into the plaintext multiplication. I can't build new plaintexts from constants, but I can rotate the ciphertext. Rotating the indicator costs rotations too.
- Alternative: reduce the sum of the three products with different shifts. Let q = p_0 + (p_1 >> 1) + (p_2 >> 2). Then q[s] = p_0[s] + p_1[s-1] + p_2[s-2]. A full reduction would mix all three columns, so it doesn't work directly.

Better: reduce with a log-step tree that keeps the columns separate. After the first reduction steps, the data is folded into fewer distinct slots. Combine at the end:
- Do the reduction down to a block of 4 slots (the steps 8192 … 4, which is 11 rotations per column). At that point slot k mod 4 holds a partial sum for each column.
- Then the final 2 steps (2 and 1) would complete the sum. Placing the columns before those steps requires masks, which I can't create (only first_slot is available).

Cheaper practical option: do the reduction per column, but share the placement. Reduce each column fully (13 rotations each, in every slot). Then the output is c0*first_slot + (c1*first_slot >> 1) + (c2*first_slot >> 2). The rotations on the masked values cost 2 more. Alternatively, rotate before masking: r = c0 + (c1 >> 1) + (c2 >> 2), but then the mask no longer isolates a single slot. Since every slot of c_i holds the same value, r[0] = c0 + c1 + c2, which is wrong.

Use the reduction's linearity instead. Reduce A = p_0 + (p_1 >> 1) + (p_2 >> 2) with the full rotation tree. That gives a total T = S0 + S1 + S2 in every slot, which is useless on its own.

I'll keep the structure but cut the mask multiplications. Masking and rotating costs 3 ct_pt mults and 2 rotations. Do the mask once instead: output = first_slot * (c0 + (c1 >> 1) + (c2 >> 2)) is wrong, as shown above.

A practical saving is to defer the mask multiplication. With the mask applied to the sum of the shifted values, slot 0 gets c0 + c1 + c2, which is wrong. So instead I fold the mask into the first multiplication: use the same sequence on the three products, then take m_i = c_i * first_slot. That is the baseline, and I'll keep it with one tweak. Rotating each p_i first in log steps brings no gain. I'll submit the baseline's reductions with the final combination reordered, to try a different circuit.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  // shift first (rotate by i), then one combined mask is not valid; mask each.
  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
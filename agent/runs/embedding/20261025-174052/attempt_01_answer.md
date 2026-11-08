The baseline does 3 full rotate-and-add reductions (3×14 = 42 rotations nominally; CHEHAB reports 26). We can cut this by packing the three products before reducing.

Idea: the reduction is linear, so first mask and position, then reduce once. Let P_i = indicator * table_i. Combine them as P_0 + (P_1 >> 1) + (P_2 >> 2) into a single vector, then one reduction would sum across all slots, mixing slots. That doesn't work directly, because the rotation by 1 shifts every slot.

Instead, fold the table positioning into the plaintext. Rotating the plaintext is free (it is not a ciphertext operation), but plaintext-plaintext ops are not allowed and the rotation can't be applied to a plaintext input at compile time. So that route is out.

Alternative: reduce the three vectors with a shared tree. Reduction by sum over all slots gives the total in every slot. Sum the three products after masking them into distinct slot sets? The masks would need constants, and only first_slot is available.

A cheaper method is to reduce the first stages jointly. Define a = P_0, b = P_1, c = P_2. A rotate-and-add step on all three costs 3 rotations. Combining the three before the reduction would blend them, so the answer must keep them separate.

Trick: after the full reduction every slot holds the total T_i. The output needs T_0 in slot 0, T_1 in slot 1, T_2 in slot 2. Build the output before the last reduction steps instead:
- Compute r_i = P_i, and reduce each over the large strides down to stride 1 (that is, the first 13 steps).
- Now each slot j of r_i holds a partial sum, and the full total needs a last step of adding the slot at offset 1... 

This still has the same cost per column, so I'll keep the structure and only reduce the mask work. The mask multiplications can be merged: the reduction result is the same in every slot, so mask the sum via a single multiplication, as in (c0 + (c1 >> 1) + (c2 >> 2)) * first_slot. That is wrong, because a shifted c1 has the total in all slots and slot 0 would receive c0+c1+c2. So the mask must be applied per term, as in the baseline.

Better saving: do the masking before the final reduction steps, since the reduction commutes with a slot-pick only at the end. This gives no gain.

I'll do the safe improvement: mask each coordinate before shifting, which is the baseline, but combine with a shared structure. With no further safe gain, I'll submit a version that delays the shifting: sum coordinates as m_0 + ((m_1 + (m_2 >> 1)) >> 1), which uses 2 rotations as before but ordered the same. That doesn't change the circuit, so it won't win, yet it is correct.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> m;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    m.push_back(r * first_slot);
  }
  Ciphertext inner = m[1] + (m[2] >> 1);
  Ciphertext output = m[0] + (inner >> 1);
  output.set_output("output");
}
```
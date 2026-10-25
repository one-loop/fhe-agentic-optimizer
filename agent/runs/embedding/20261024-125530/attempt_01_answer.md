Idea: the baseline does 3 full rotate-and-add reductions (3×13 = 39 rotations, though CHEHAB reports 26 after simplification). Instead, combine the three columns first so that only one reduction is needed.

Mask each product to its target slot before reducing: the output needs sum_j ind[j]*table_i[j] in slot i. Let P_i = ind*table_i. Then:
- Form Q = P_0 + (P_1 >> 1) + (P_2 >> 2). This costs 2 rotations.
- Reduce Q with a full rotate-and-add (13 rotations). Every slot then holds sum_k Q[k], which mixes the columns, so that does not work directly.

A better approach is to keep the columns separate through the reduction by using plaintext masks, but the only available mask is first_slot. A full reduction leaves each sum in all slots, so one reduction per column is the baseline structure. To merge them, I pre-rotate the table plaintexts instead. Rotating plaintexts is plaintext-only work, and the DSL doesn't allow it without a ciphertext operand.

Alternative: reduce over blocks. Use the rotate-and-add reduction on a combined vector, but only over the offsets that sum across slots, leaving the columns in distinct positions.
- Let S_i = sum_j P_i[j].
- Define T = P_0 + (P_1 >> 1) + (P_2 >> 2). Then T[k] = P_0[k] + P_1[k-1] + P_2[k-2].
- The total sum over k of T is S_0+S_1+S_2, which mixes the columns, so this fails.

Cheaper exact trick: reduce the three columns with shared rotations. Rotations are linear, so R(P_0) + R(P_1) + R(P_2) is not separable. But the reduction's last steps can be shared if the columns are first folded into different slots. Fold each P_i with steps from slots()/2 down to 2 (12 rotations each), then each slot holds the sum over its parity class. This is still 36 rotations, so there's no gain.

I keep the baseline's per-column structure but reduce the mask cost. Since the baseline already runs at 26 rotations after CHEHAB's simplification, I only apply one safe improvement: mask after combining. The columns are masked and shifted after reduction, so I do the masking once on the sum:

output = (r0 + (r1 >> 1) + (r2 >> 2)) * first_slot is wrong, because the shifted r1 has the value in every slot, not only slot 1. So the masking must be per column and positioned correctly.

Instead, I use a mask built from first_slot: m1 = first_slot >> 1 needs a ciphertext. Because of that I reduce the multiplications by combining first. Compute A = r0*first_slot, and for the others use (r1 >> 1) + (r2 >> 2) with masks. This saves nothing substantial, so I submit the baseline form with the multiplications combined where possible.

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

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
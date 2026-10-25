The baseline does 3 independent rotate-and-sum reductions (8 rotations each, 24 in total).

Idea: the reduction is linear, so the 3 columns can share rotations. Rotate-and-sum cannot be merged across ciphertexts in CKKS without packing, but the masking can be exploited. After multiplying, r_i = indicator * table_i. Pack the three products into one ciphertext using disjoint slot regions, then reduce all of them together. A full cyclic sum over all slots mixes regions, though, so this needs care.

Alternative: first reduce within blocks. Each of the 3 products is a full 16384-slot vector, so there is no free space to pack them into.

A cheaper approach uses the first reduction steps. Let s_i = r_i. Step 1 for each column uses a rotation by 8192. Combine columns with plaintext masks: t = (r_0 + r_1<<8192 masked…). This costs extra plaintext multiplies and gets complicated, so I'll use a simpler trick.

Fold: for the columns a, b, c, form u = mask_lo*a + mask_hi*(b rotated) so that u holds the folded halves of two columns. Masks must come from plaintext inputs, though, and I can't create constants, so this isn't available.

Without constants, the safe saving is limited. The baseline already uses a minimal number of rotations per column, and the sum is linear, so I'll keep the structure. One real saving is that the mod-level work is cheaper if the multiply happens first (already the case). The reduction rotations run at a deeper level anyway.

I'll make a small, safe change: reduce the rotation count by having rotations by 1 happen on a combined ciphertext. I can't do that without masks, so I return the baseline with the loop order interleaved, which gives the same cost.

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
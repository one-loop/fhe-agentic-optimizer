The baseline does 3 plaintext multiplies and 3 full rotate-and-sum reductions of 8 rotations each, 24 rotations in total.

The key observation is that rotation is linear. Instead of reducing each product separately, I can pack the three products into one ciphertext and reduce them together. This needs masks, and masks would have to be plaintext constants, which I can't create. The inputs are only the tables.

A different route avoids masks. Let the slot count be S = 16384 and let r_i = indicator*table_i. The target is a total sum for each i in every slot, so a mask-free packing isn't possible.

Another option is to reduce the work with the same total-sum structure, since the sum over all slots is invariant under rotation. Combining the r_i with rotations would still need a mask to separate the outputs, so this doesn't help either.

Without masks the 3 reductions stay independent, and each needs log2(S) = 14 rotation steps. Only 8 appear in the baseline, which means slots() is probably 256 and the 8 steps cover 128 down to 1.

So the remaining saving is in the plaintext side. The sum of indicator*table_i over slots can be reorganized as follows:

1. Rotate the indicator first, then multiply by rotated tables. That needs rotated plaintexts, which are cheap to encode but still cost multiplications, so it is no better.
2. Reduce the rotations using the fact that the first reduction steps can be shared across outputs. Sharing needs masks to merge outputs, which I don't have.

Since I can't remove the rotations without masks, I'll try a different formulation. I'll use a plaintext combination, with table_0 + t1*... Constants aren't available, so that fails too.

The only safe change is to reorder the operations. The result is the same circuit, so it can't win. I'll submit a variant that changes the circuit: compute the rotations of the indicator first (8 rotations, shared), then multiply each rotated indicator by the plaintext table rotated the opposite way. Rotating plaintexts isn't allowed, though, since they are inputs and a rotation needs a ciphertext.

I'll go with the baseline structure, since I can't find a valid improvement that stays within the rules. A small circuit change is to sum in a different order, doing the reduction with fewer adds, but the operation counts remain the same.

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
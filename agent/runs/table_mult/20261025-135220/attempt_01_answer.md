Each output needs a full sum over all slots of indicator*table_i, which costs 14 rotations per output with the baseline's rotate-and-add (24 rotations are reported, so the slot count is likely 2^8 or similar). The rotations dominate the cost, so I want to share them across the three outputs.

The idea is to pack the three products into one ciphertext before the reduction. Masks would be needed to do that, and rule 1 says constants must come from plaintext inputs, so I cannot build masks myself. The packing trick is therefore unavailable.

The reduction is linear, so there is another option. Rotate-and-add on r0, r1 and r2 separately costs 3×8 = 24 rotations. The slot count is probably 256, since 8 steps per output gives 24 rotations. Without masks I cannot combine the three, so the number of rotations cannot be reduced that way.

What I can do is change the order of operations. Rotating the indicator and multiplying by rotated plaintexts would need plaintext rotation, which is not allowed because it is plaintext arithmetic. So I will keep the structure and make one small change: sum the products with a different rotation order, which may give a different circuit but the same counts. That cannot win, so the baseline is effectively the best I can do without masks.

I am submitting the baseline structure with the reduction steps in ascending order. This is a safe attempt that gives a different circuit shape but the same result.

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
    for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    r.set_output("output_" + to_string(i));
  }
}
```
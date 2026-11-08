The current circuit appears to be at the minimum for this task, so I don't expect a real speedup from any kernel. A different circuit can only come from changing the dependency depth, not the operation counts.

**Why the counts can't go lower:**
- **Masks (3, so 6 ct-pt multiplies in total):** a rotate-and-add reduction is a circular convolution, so it can never zero out the other slots by itself. Every value that ends up alone in a slot has to pass through a multiply by `first_slot`. The three sums are independent numbers, so they need three masks.
- **Shift rotations (2):** every masked value sits in slot 0, and the values bound for slots 1 and 2 need different shifts. That takes at least two rotations.
- **Reduction rotations (24):** packing two columns into one ciphertext before reducing would mix their sums. Keeping them apart would need block masks, and the only mask available is `first_slot`.
- **Levels (2):** one level for the table multiply and one for the mask. The two can't be merged, because that would need plaintext-plaintext arithmetic, which is not allowed.

**This attempt:** I run each reduction with ascending steps (1, 2, ..., slots()/2) and combine the final terms as `(M0 + (M2 >> 2)) + (M1 >> 1)`. The full sum still lands in every slot, so the result is unchanged. It is a different, flatter grouping than attempt 1. If it times about the same, the baseline is effectively optimal.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> masked;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    masked.push_back(r * first_slot);
  }

  Ciphertext output = (masked[0] + (masked[2] >> 2)) + (masked[1] >> 1);
  output.set_output("output");
}
```
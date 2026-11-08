The baseline is already close to the minimum for this task, so I made only a small change to the last step.

**Why the main costs stay:**
- **Rotations:** each column's sum needs its own log2(slots()) rotate-and-add steps. Merging the columns would need block masks, and the only mask available is `first_slot`.
- **Masks:** the three reduced sums are constant across all slots. Each needs its own multiply by `first_slot` to clear the other slots, which also fixes the depth at two levels.
- **Placement:** the masked values then need two shifts to land in slots 1 and 2.

**The change:** I nest the last combination, `output = m0 + ((m1 + (m2 >> 1)) >> 1)`. The operation counts stay the same: 3 masks and 2 shift rotations. Both rotations now act on ciphertexts at the lowest level, and the second one is shared between coordinates 1 and 2. This gives a different circuit, but any speedup will be small.

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
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    masked.push_back(r * first_slot);
  }

  Ciphertext tail = masked[1] + (masked[2] >> 1);
  Ciphertext output = masked[0] + (tail >> 1);
  output.set_output("output");
}
```
The 1464 ms result for this resubmission is timing noise, since the kernel is identical to attempt 1 (1285 ms). I could not find a valid structural improvement, so I am resubmitting the attempt 1 kernel.

Why nothing better is available under these rules:
- Each coordinate is a full sum over all slots, and each sum needs log2(slots) rotations on its own ciphertext. Placement before the reduction does not work: a rotation leaves the total unchanged, and masking before the tree keeps only one slot.
- Packing three coordinates into shared lanes needs lane masks. The only mask I can build is `first_slot` (one slot), because plaintext-plaintext operations are forbidden and table columns cannot be combined into masks.
- Rotating the tables at compile time would let rotations be shared across columns, but that depends on plaintext rotation being allowed, which the rules do not state. I have not tried it.

```cpp
// Embedding lookup: each column's total is computed with a rotate-and-add tree
// (every slot holds the total). The three totals are then packed into slots
// 0,1,2 with a Horner scheme: out = ((S2*f)>>1 + S1*f)>>1 + S0*f, where f is
// first_slot. This uses 2 rotations for placement and 3 ciphertext-plaintext
// masks.
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> totals;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    totals.push_back(r);
  }

  Ciphertext acc = totals[2] * first_slot;
  acc = (acc >> 1) + (totals[1] * first_slot);
  acc = (acc >> 1) + (totals[0] * first_slot);
  acc.set_output("output");
}
```
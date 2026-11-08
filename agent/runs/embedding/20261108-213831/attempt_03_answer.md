I couldn't find a cheaper circuit. Every operation count in the baseline appears to be at its minimum, so any kernel I send now would be a reshuffle of the same operations.

The reasons, using only the operations this task allows:

- **Masks:** at least 3 multiplies by `first_slot`. Rotations and additions alone compute a circular convolution. No convolution maps every input to something nonzero in only slots 0–2, so each output value must come from a slot-0 mask. The three outputs are independent numbers, so they need three masks. Merging a mask with the table multiply would be plaintext-plaintext arithmetic, which is forbidden. Together with the 3 table multiplies, that gives 6 ct-pt multiplies, 6 rescales and 2 levels.
- **Shift rotations:** at least 2. Every mask leaves its value in slot 0, and the values bound for slots 1 and 2 need two different shifts.
- **Reduction rotations:** at least log2(slots()) per column, 24 in total. Each rotate-and-add at most doubles how many inputs a slot depends on. Two columns can't share rotations: packing them into one ciphertext needs block masks, and sum/difference tricks need a constant 1/2, which the CKKS rules forbid.

That totals 26 rotations, 6 ct-pt multiplies and 2 levels, exactly the baseline. Attempt 1 regrouped the same operations, and attempt 2 matched the baseline exactly. The only remaining freedom is grouping, which CHEHAB either normalizes away or turns into a deeper circuit.

The kernel below is the baseline, which I consider optimal for this task:

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

  Ciphertext output = masked[0];
  for (size_t i = 1; i < masked.size(); ++i)
    output += masked[i] >> static_cast<int>(i);
  output.set_output("output");
}
```
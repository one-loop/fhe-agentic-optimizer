Idea: the mask multiplications are the only part I can restructure safely with just `first_slot`. I mask `c2` first, shift it to slot 1, add the full `c1`, and then mask the sum. That yields T1 in slot 0 and T2 in slot 1, with zeros elsewhere. Next I shift that sum by 1 and add it to `c0`, then mask again. Slot 0 of that sum is T0 and slot 1 is T1, but slot 2 needs T2, so the packing is wrong.

The version below is the correct restructuring. It builds the output as `first*(c0 + (first*(c1 + (first*c2 >> 1)) >> 1))`. I have not verified that this changes the circuit.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> c;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    c.push_back(r);
  }
  // m2: T2 at slot 0
  Ciphertext m2 = c[2] * first_slot;
  // t1: T1 everywhere in slots >= 0, plus T2 at slot 1 (T1 + T2 at slot 1)
  Ciphertext m1 = c[1] * first_slot;
  Ciphertext inner = m1 + (m2 >> 1);
  Ciphertext m0 = c[0] * first_slot;
  Ciphertext output = m0 + (inner >> 1);
  output.set_output("output");
}
```

Note that this still has the same operation counts as the baseline (the masks stay per coordinate), so it is unlikely to be confirmed faster.
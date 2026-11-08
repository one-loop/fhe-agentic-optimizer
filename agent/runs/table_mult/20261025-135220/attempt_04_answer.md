I can't find a cheaper circuit, and the last attempt failed. The transparent-ciphertext error came from CHEHAB cancelling `a - b - c` against its own terms.

I also checked a packing trick: fold part-way, with `r1` shifted into the other half of the slots. It cannot work without 0/1 masks. Any combination of unmasked rotations of `r0`, `r1`, `r2` only exposes sums like S0+S1 and never S0 alone, because every slot of each product contributes with coefficient 1. The three scalar reductions therefore stay independent at 8 rotations each.

The best circuit remains the baseline. I'm resubmitting it with the reduction done as a shared lambda over a tree of partial sums, which gives the same counts and is correct.

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
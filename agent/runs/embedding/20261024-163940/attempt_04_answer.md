I haven't found a circuit that beats the baseline, and I'd rather say so than send a kernel that changes nothing real.

I looked at four ways to share work across the three columns:
- **Packing columns into one vector:** the totals become a mixture of classes that can't be separated without masks.
- **Combining totals** (T0+T1, T0−T1, …): this still needs three independent reductions.
- **Stopping the reduction early and masking per class:** this needs more masks than the baseline uses.
- **Merging the three placement masks into one:** the reduced values are uniform across slots, so each needs its own `first_slot` mask.

The only mask available is `first_slot`, and each of these fails for that reason. Here is the kernel again with the lowest-risk ordering, which keeps the baseline's operation counts.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext r0 = indicator * t0;
  Ciphertext r1 = indicator * t1;
  Ciphertext r2 = indicator * t2;
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    r0 += r0 << static_cast<int>(steps);
    r1 += r1 << static_cast<int>(steps);
    r2 += r2 << static_cast<int>(steps);
  }
  Ciphertext out = r0 * first_slot;
  out += (r1 * first_slot) >> 1;
  out += (r2 * first_slot) >> 2;
  out.set_output("output");
}
```
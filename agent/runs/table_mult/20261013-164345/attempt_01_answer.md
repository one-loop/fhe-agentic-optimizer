The rotation-sum over all slots is the dominant cost, and each output needs its own full sum. Packing the three columns into one ciphertext would require plaintext masks, which the DSL cannot create from constants, and plaintext-plaintext arithmetic is disallowed. Every output therefore still needs its own log2(slots) rotation tree, so this kernel keeps the baseline structure. It only makes the tree explicit and avoids the extra column-loop bookkeeping, so I expect little or no speedup.

```cpp
// Each output is a full slot sum of indicator * table_i, so it needs its own
// log2(slots) rotation-and-add tree. Packing columns needs plaintext masks,
// which are not expressible here, so the three trees are kept separate.

void fhe()
{
  Ciphertext indicator("indicator");

  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");

  Ciphertext r0 = indicator * t0;
  Ciphertext r1 = indicator * t1;
  Ciphertext r2 = indicator * t2;

  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    r0 += r0 << static_cast<int>(steps);
    r1 += r1 << static_cast<int>(steps);
    r2 += r2 << static_cast<int>(steps);
  }

  r0.set_output("output_0");
  r1.set_output("output_1");
  r2.set_output("output_2");
}
```
I don't have a further structural improvement. Every route I checked ends at the same minimum:

- three independent reductions, since the three totals are unrelated combinations and no extra masks exist;
- three `first_slot` masks, because the reduced values are uniform across slots;
- two placement shifts, one per extra slot.

The rotation count of 26 (3 × 8 + 2) is therefore at the floor for these primitives. Reordering, regrouping, Horner-style chains and early masking all left the circuit unchanged or made it worse. The rotation and multiply counts can't drop below what the baseline already has, so I'm submitting the same best-known correct kernel again.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext r0 = indicator * t0;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r0 += r0 << static_cast<int>(s);
  Ciphertext r1 = indicator * t1;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r1 += r1 << static_cast<int>(s);
  Ciphertext r2 = indicator * t2;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r2 += r2 << static_cast<int>(s);

  Ciphertext out = r0 * first_slot;
  out += (r1 * first_slot) >> 1;
  out += (r2 * first_slot) >> 2;
  out.set_output("output");
}
```
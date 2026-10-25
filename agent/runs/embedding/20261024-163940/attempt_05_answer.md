I still haven't found a circuit with fewer operations than the baseline. I checked the lower bounds and the baseline's structure looks minimal given the available primitives:

- **Masks:** only `first_slot` exists, so the final vector can only be zeroed outside slot 0 by a `first_slot` multiply. Each of the three totals needs its own multiply (3 masks) and two placement shifts.
- **Reductions:** the three totals are independent, and packing columns before reducing loses information, so three reductions are needed.
- **Rotation count:** this already matches the 26 rotations reported (3 × 8 + 2).

I'm resubmitting the best-known correct kernel. It will tie the baseline's circuit and cannot win, but it avoids a wrong answer.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
  {
    Plaintext col("table_" + to_string(i));
    Ciphertext x = indicator * col;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    r.push_back(x);
  }
  Ciphertext out = r[0] * first_slot;
  out += (r[1] * first_slot) >> 1;
  out += (r[2] * first_slot) >> 2;
  out.set_output("output");
}
```
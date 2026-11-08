I found no cheaper circuit than the baseline after several structural attempts. Each of the three columns needs its own log-depth reduction, and the only mask available is `first_slot`, so the columns cannot share rotations. The two output shifts and the three mask multiplications are also the minimum with that single mask. Re-expressing the same computation, as in attempts 1–3, produced identical circuits, and the S − T0 − T1 variant and the direction-swapped variant were slower.

I'm submitting the baseline structure again. Judged against it, it should not register as an improvement.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```
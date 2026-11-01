The baseline spends 24 rotations, 8 per column, on rotate-and-add reductions, and that accounts for almost all of the 560 ms. I looked for a way to share rotations across the three columns. Packing them into one ciphertext needs slot-separating masks, and this DSL provides no way to build masks from constants or from the tables. Packing with powers of two would lose the precision needed for 1e-4 accuracy. So I could not find a sound change that reduces the rotation count, and the kernel below keeps the baseline's structure. It is correct but is not a speedup.

```cpp
// Same structure as baseline: one indicator*table_i product per column (depth 1),
// then a log2(slots) rotate-and-add reduction so every slot holds the total.
// Rotation count (log2(slots) per column) cannot be reduced without slot-separating masks.

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
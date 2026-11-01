I'm submitting the attempt-3 kernel again: it measured 559.91 ms, the best so far, and the other variants fall within noise of the baseline (560.0–560.6 ms).

I have not found a real speedup. The 24 rotations (8 per output column) dominate the cost, because each output is a full sum over the slots and one rotate-and-add reduction yields one scalar. Sharing reductions across the three columns would need slot-separating masks that this DSL cannot build from constants or the table inputs.

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
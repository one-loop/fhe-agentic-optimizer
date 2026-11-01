I'm submitting the attempt-3 kernel again. It is the best measured at 559.91 ms, and the other variants measured 560.0–560.3 ms, which is noise. I have not found a real speedup.

The 24 rotations (8 per output column) dominate the cost. Each output is a full sum over the slots, and a rotate-and-add reduction turns one ciphertext into one scalar in every slot. Three independent sums therefore need about three reductions, and sharing them would require slot-separating masks that this DSL cannot build from constants or the table inputs. Pre-rotating the plaintext tables would avoid this, but the rules only describe rotating ciphertexts, so I did not rely on it.

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
The best measured time is still the baseline, about 560 ms (attempt 3 measured 559.91 ms, within noise). I could not find a rotation-saving packing that the DSL supports, so I'm stopping here rather than spending more attempts on variants.

The 24 rotations (8 per column) are the cost, and I see no way around them. A single rotate-and-add reduction produces one scalar per ciphertext, so three independent sums need about three reductions. Separating the columns into slot blocks would need masks that this DSL can't build from constants or the table inputs. Pre-rotating the tables is the one idea left, but the rules only describe rotating ciphertexts, so I did not rely on it.

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
I'm stopping here and not spending the remaining attempts. The kernel below is the attempt-3 version, the best measured at 559.91 ms. Every variant I tried measured within noise of the baseline (559.9–560.3 ms), so the differences are measurement noise, not a real speedup.

The cost is 24 rotations, 8 per output column. Each output is a full sum over the slots, and a rotate-and-add reduction turns one ciphertext into one scalar in every slot. Packing the three columns into separate slot blocks would need slot-separating masks, which this DSL cannot build from constants or the table inputs. I also considered pre-rotating the plaintext tables, but the rules only describe rotating ciphertexts, so I did not rely on that.

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
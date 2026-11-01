I still have no kernel that beats the baseline. This attempt measured 560.19 ms, the same as before within noise.

The problem is structural. Each output is a full sum over 256 slots, and a rotate-and-add reduction turns one ciphertext into one scalar in every slot. Any reduction of a linear combination of the products gives only one linear combination of the three sums, so three independent sums need roughly three reductions and about 24 rotations. Packing the columns into separate slot blocks would need masks, and this DSL gives no way to build them from constants or from the table inputs. Rotating the plaintext tables in advance would be the other route, but the rules only describe rotating ciphertexts, so I did not rely on it.

I am submitting the baseline structure again.

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
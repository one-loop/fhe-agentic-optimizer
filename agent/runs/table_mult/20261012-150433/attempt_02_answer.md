I could not find a sound way to cut the rotation count, so this attempt keeps the baseline structure. The result is about the same time (560.45 ms vs 560.02 ms), within noise.

The cost is dominated by 24 rotations (8 per column). Each column needs its own all-slot sum, and a single rotate-and-add reduction yields one scalar per ciphertext. Packing the three columns into one ciphertext would need slot-separating masks, and this DSL gives no way to build masks from constants or from the table inputs. Packing with powers of two (via doubling) would lose the 1e-4 accuracy needed. Rotating plaintext tables in advance is not an available operation as far as I know. Moving rotations to lower levels doesn't help because the rotated products are already at the deepest level used.

I have not found a route below about 24 rotations, so I'm stopping here with the baseline-equivalent kernel.

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
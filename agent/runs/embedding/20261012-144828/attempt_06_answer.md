I have not found a kernel that is meaningfully faster than the baseline, so this resubmits the best kernel so far (attempt 3, 615.82 ms, within noise of the baseline's 616.80 ms).

The cost is almost all rotations: 26 at about 20 ms each. Every variant I tried still needs a full rotate-and-add reduction per column, because one such reduction yields only one scalar sum. Packing the three columns into one reduction would need slot-range masks, and the only plaintext mask available is `first_slot` (slot 0 only). Folding the vectors first also needs half-range masks, so it doesn't help either. I have three attempts left but no concrete idea that avoids the per-column reductions, so I am stopping here rather than spending them on untested variants.

```cpp
// Same three full rotate-and-add reductions as the baseline (each column's
// sum lands in every slot). The output is assembled as
//   out = S0*first + ((S1*first + (S2*first)>>1) >> 1)
// which places S0 in slot 0, S1 in slot 1, S2 in slot 2 with the same
// number of multiplications and rotations as the baseline's assembly.
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> sums;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    sums.push_back(r);
  }

  Ciphertext s2 = (sums[2] * first_slot) >> 1;
  Ciphertext s1 = (sums[1] * first_slot) + s2;
  Ciphertext output = (sums[0] * first_slot) + (s1 >> 1);
  output.set_output("output");
}
```
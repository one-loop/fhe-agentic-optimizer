The last two attempts did not improve on the baseline, and I don't see a reduction-based kernel that would. Every output needs its own full sum over all slots, and a single rotate-and-add tree yields only one such sum per ciphertext. Separating three columns inside one tree would need slot-selective plaintext masks, and the only plaintext available is `first_slot`, so I can't build them. Under these constraints the baseline's three log2(N) reductions are close to minimal. I'm resubmitting the best kernel so far (attempt 1, 616.22 ms) and am not claiming a speedup.

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
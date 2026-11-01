I could not find a kernel that is meaningfully faster, so this resubmits the best kernel so far (attempt 3, 615.82 ms, a 0.2% gain that is within noise).

The cost is almost entirely rotations (26 × ~20 ms). Each full rotate-and-add tree produces one scalar sum in every slot. Three independent column sums need three such functionals. Partial trees produce class sums, but separating the columns into different slots needs slot-selective plaintext masks, and the only one available is `first_slot` (slot 0 only). Plaintext rotation of masks would be the way to get more, but CHEHAB likely rejects it, and I have no evidence it works. The remaining operations (the three `first_slot` multiplies and the shift adds) cost about 5 ms each and cannot save anything substantial.

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
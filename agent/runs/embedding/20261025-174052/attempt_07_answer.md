I still see no way to beat the baseline. Every cheaper packing I considered needs masks other than `first_slot`, or halving, which CKKS cannot do without constants. This attempt computes the columns triangularly as A = P0, B = P0+P1, C = P0+P1+P2. It runs the same number of reductions, so I expect it to be no faster.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  auto reduce = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };

  Ciphertext p0 = indicator * columns[0];
  Ciphertext p1 = indicator * columns[1];
  Ciphertext p2 = indicator * columns[2];
  Ciphertext a = reduce(p0);
  Ciphertext b = reduce(p0 + p1);
  Ciphertext c = reduce(p0 + p1 + p2);

  // T0 = a, T1 = b - a, T2 = c - b
  Ciphertext m0 = a * first_slot;
  Ciphertext m1 = (b - a) * first_slot;
  Ciphertext m2 = (c - b) * first_slot;
  Ciphertext y = (m2 >> 1) + m1;
  Ciphertext output = (y >> 1) + m0;
  output.set_output("output");
}
```
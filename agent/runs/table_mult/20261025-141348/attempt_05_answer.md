I haven't found a cheaper circuit. Every route to fewer rotations needs masks to separate the three columns, and none can be built from the allowed inputs. The identity-based variants (attempts 2 and 4) only shuffle additions and ran at parity with the baseline. I'm submitting the same kind of variant once more: reduce p0 and p1+p2 as the two shared reductions, and recover output_1 and output_2 by subtraction. The rotation count stays at 24, so I don't expect it to win.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  Ciphertext p0 = indicator * columns[0];
  Ciphertext p1 = indicator * columns[1];
  Ciphertext p2 = indicator * columns[2];
  auto red = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };
  Ciphertext s1 = red(p1);
  Ciphertext s2 = red(p2);
  Ciphertext q = p0 + p1;
  q += p2;
  Ciphertext sq = red(q);
  Ciphertext s0 = sq - s1;
  s0 -= s2;
  s0.set_output("output_0");
  s1.set_output("output_1");
  s2.set_output("output_2");
}
```
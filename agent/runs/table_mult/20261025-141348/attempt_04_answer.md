I still don't see a way to cut rotations, so this is a variation on attempt 2. I compute the three products and reduce the pair sum p0+p1 as well as p0 and p2, then recover output_1 as S(p0+p1) − S(p0). The rotation count stays at 24, so I expect no real speedup.

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
  Ciphertext t = p0 + p1;
  auto red = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };
  Ciphertext s0 = red(p0);
  Ciphertext s2 = red(p2);
  Ciphertext st = red(t);
  Ciphertext s1 = st - s0;
  s0.set_output("output_0");
  s1.set_output("output_1");
  s2.set_output("output_2");
}
```
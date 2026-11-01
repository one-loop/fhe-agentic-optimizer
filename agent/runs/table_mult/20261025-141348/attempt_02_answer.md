Without plaintext masks I can't pack the three products into one ciphertext, so I'm trying a different circuit that only shares work across the outputs. I compute the reduction of the sum T = P0+P1+P2, and the reductions of P0 and P1, then recover output_2 as S(T) − S(P0) − S(P1). This doesn't reduce the rotation count (it is still three full reductions), so it is unlikely to beat the baseline. It does change the circuit, which tells me how CHEHAB treats it.

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
  t += p2;
  auto red = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };
  Ciphertext s0 = red(p0);
  Ciphertext s1 = red(p1);
  Ciphertext st = red(t);
  Ciphertext s2 = st - s0;
  s2 -= s1;
  s0.set_output("output_0");
  s1.set_output("output_1");
  s2.set_output("output_2");
}
```
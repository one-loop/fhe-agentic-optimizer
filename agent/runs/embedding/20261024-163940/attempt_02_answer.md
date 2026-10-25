The idea is to cut the placement rotations from three to two. Before this, the baseline shifted r_1 by 1 and r_2 by 2 separately. This version builds a Horner-style chain of masked values, so each shift is applied once and carries the earlier terms along: y = ((r_2·fs) >> 1 + r_1·fs) >> 1 + r_0·fs. The reductions are unchanged. I haven't found a way to share the reduction itself, because the only available mask is `first_slot`.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
  {
    Plaintext col("table_" + to_string(i));
    Ciphertext x = indicator * col;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    r.push_back(x);
  }
  Ciphertext y = r[2] * first_slot;
  y = y >> 1;
  y += r[1] * first_slot;
  y = y >> 1;
  y += r[0] * first_slot;
  y.set_output("output");
}
```
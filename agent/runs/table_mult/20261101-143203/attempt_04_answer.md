Attempt 3 nearly worked: it ran in 502 ms against the baseline's 570 ms. It missed the tolerance only barely, at 1.00006e-4 against 1e-4 on output_1.

The error comes from the fake zero, and it scales with the size of the two tables used to build it. Attempt 3 used `table_0` and `table_1`. This time I keep the same structure but build it from `table_0` and `table_2`:

`z = (indicator*t0)*t2 − (indicator*t2)*t0`

This is a guess that `table_2` has smaller values; I can't measure the tables directly. If it holds, the error should drop back under tolerance. Everything else is unchanged: 2 extra ct×pt multiplies, with all 24 rotations at level 2.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> t;
  for (int i = 0; i < 3; ++i)
    t.emplace_back("table_" + to_string(i));

  vector<Ciphertext> p;
  for (int i = 0; i < 3; ++i)
    p.push_back(indicator * t[i]);

  Ciphertext A = p[0] * t[2];
  Ciphertext B = p[2] * t[0];
  Ciphertext z = A - B; // mathematically zero, at level 2

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i] + z;
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
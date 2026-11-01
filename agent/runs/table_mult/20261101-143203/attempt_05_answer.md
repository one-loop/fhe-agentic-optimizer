Attempt 4 crashed with "result ciphertext is transparent". The likely cause is that `table_2` is all zeros in the fixed test case. Then `(indicator*t2)*t0` is an exact zero, which SEAL rejects. So the fake zero has to avoid `table_2` and go back to `z = p0*t1 − p1*t0`. Attempt 3 showed that version runs and is about 12% faster, but it missed the tolerance by a hair.

To cut the error from `z`, each output now does its first two reduction steps at level 1 without `z`: `r += r<<128`, then `r += r<<64`. Only after that is `z` added, and the remaining 6 steps run at level 2.

With this order, `z`'s per-slot noise is summed over 64 slots instead of 256. Assuming that noise is independent from slot to slot, this roughly halves its contribution to the error, which should land around 6e-5. The cost is 6 of the 24 rotations staying at level 1. By my estimate the 18 cheaper rotations still save about 50 ms, roughly 9%.

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

  Ciphertext A = p[0] * t[1];
  Ciphertext B = p[1] * t[0];
  Ciphertext z = A - B; // mathematically zero, at level 2

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i];
    int steps = slots() >> 1;
    for (int k = 0; k < 2 && steps > 0; ++k, steps >>= 1)
      r += r << steps;
    r = r + z;
    for (; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
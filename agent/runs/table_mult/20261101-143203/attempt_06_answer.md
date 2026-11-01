Attempt 5 passed, with a maximum error of 4.2e-5, so there is room to push `z` earlier. This time each output does only one reduction step at level 1 (`r += r<<128`) before `z` is added. The other 7 rotations per output, 21 in total, then run at level 2, where they are cheaper.

On error:
- `z` is now summed over 128 slots instead of 64. Assuming its noise is independent per slot, its contribution grows by about √2.
- From attempts 3 and 5, I estimate the total error at roughly 7–8e-5. That is under the 1e-4 tolerance, but not by much.

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
    r += r << steps;
    steps >>= 1;
    r = r + z;
    for (; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```
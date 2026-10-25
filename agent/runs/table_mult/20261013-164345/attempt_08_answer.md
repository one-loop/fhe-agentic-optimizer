I'm submitting the same kernel once more; it measured 1172.19 ms, effectively tied with the best (1172.21 ms in attempt 4). I did not find a change that reduces the 24 rotations, so there is no reason to expect a different result from a further attempt.

```cpp
// Each output is a full slot sum of indicator * table_i, so it needs its own
// rotation-and-add tree of log2(slots) rotations. With only ct-pt multiply,
// rotation and add available (no constants or masks), this is near the floor.

void fhe()
{
  Ciphertext indicator("indicator");

  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");

  Ciphertext r0 = indicator * t0;
  Ciphertext r1 = indicator * t1;
  Ciphertext r2 = indicator * t2;

  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    r0 += r0 << static_cast<int>(steps);
    r1 += r1 << static_cast<int>(steps);
    r2 += r2 << static_cast<int>(steps);
  }

  r0.set_output("output_0");
  r1.set_output("output_1");
  r2.set_output("output_2");
}
```
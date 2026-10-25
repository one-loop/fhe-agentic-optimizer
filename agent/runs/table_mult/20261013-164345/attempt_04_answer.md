The last three runs of essentially the same kernel landed between 1203 and 1251 ms, so the differences are timing noise rather than real changes. I could not find a structural improvement. Every output needs a full slot sum, and one tree gives one scalar in all slots, so 3 × log2(slots) = 24 rotations seems to be the floor with these primitives. I'm submitting the attempt 1 kernel as the best measured one.

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
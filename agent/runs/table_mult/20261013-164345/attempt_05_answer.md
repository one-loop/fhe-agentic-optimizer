This run is the same kernel as before. Its 1172 ms is within the noise band of the earlier runs (1203–1251 ms), so it doesn't show a real improvement. I don't see a structural change that cuts the 24 rotations: each output needs its own full slot sum, and I can't pack the three sums without masks or constants. I'm keeping this kernel as the final answer.

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
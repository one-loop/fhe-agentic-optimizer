This run used the same kernel again and came in at 1263 ms, which is within the noise band of the earlier identical runs (1172–1263 ms). Across six runs I haven't found a structural change that reduces the 24 rotations. Each output is a full slot sum, one tree yields one scalar in all slots, and packing needs masks or constants that the DSL does not provide. So I'm submitting the same kernel as the final answer, with the best measured time being 1172 ms.

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
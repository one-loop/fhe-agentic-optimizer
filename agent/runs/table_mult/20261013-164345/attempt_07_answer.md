This run is also the same kernel, at 1182 ms, inside the same noise band as the other identical runs. Resubmitting it again would not add anything, so I'm stopping here. The best measured time is 1172 ms (attempt 4), and I'm giving that kernel as the final answer.

Why I think it is near the floor: each output is a full slot sum of `indicator * table_i`. One rotation-and-add tree yields one scalar in every slot, so three outputs need three trees, which is 3 × log2(slots) = 24 rotations. Sharing a tree across outputs would require slot masks or scalar constants to separate the sums, and the DSL disallows both. Sharing rotations of the indicator across outputs (baby-step/giant-step) needs more rotations (about 60 for 256 slots), so it's worse.

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
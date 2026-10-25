Attempt 1 is the best so far at 1203 ms, so I'm keeping its structure. With 24 rotations at roughly 45–50 ms each, rotations account for almost all of the time. The DSL's primitives (ct-pt multiply, rotation, add, no constants, no plaintext-plaintext ops) don't let me pack the three outputs into one tree. Each full slot-sum tree yields a single scalar in all slots, so three independent sums need three trees. I tried to reason through windowed or half-split packings and found they still need about as many rotations, so I'm resubmitting the best kernel unchanged rather than inventing a regression.

```cpp
// Each output is a full slot sum of indicator * table_i, so it needs its own
// rotation-and-add tree. The rotation count is the cost driver, and with only
// ct-pt multiply, rotation and add available, the three sums cannot share a tree.

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
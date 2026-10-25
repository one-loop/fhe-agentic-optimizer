Attempts 1 and 2 both sit at about 1200 ms, and I don't see a structural change that beats them under these primitives. Each output is a sum over all slots, and one rotation-and-add tree of log2(slots) = 8 rotations produces one such sum in every slot. Packing the outputs into one tree needs slot masks or scaling by constants, neither of which the DSL allows. Rotating the indicator once and sharing it across outputs (baby-step/giant-step) costs more rotations (about 60 versus 24). So 24 rotations is about the floor for this formulation, and I'm resubmitting the same kernel.

```cpp
// Each output is a full slot sum of indicator * table_i, so it needs its own
// rotation-and-add tree of log2(slots) rotations. Under the available primitives
// (ct-pt multiply, rotation, add, no constants or masks) this is near the floor.

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
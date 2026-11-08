I don't think a cheaper circuit exists for this task, so this attempt is a structurally different variant that I expect to time slightly slower than the baseline.

**Why the baseline can't be beaten:**
- **Multiplications and rescales.** Each output depends on its own slot-wise product, so the 3 plaintext multiplications and 3 rescales are required.
- **No masks.** The CKKS rules forbid constants and plaintext-only operations, so no mask can be built. Without one, the three products can't be packed into one ciphertext and reduced together, and the indicator's rotations can't be shared via pre-rotated tables.
- **Rotations.** Every intermediate value is some rotation polynomial applied to each product. Each output's polynomial must be the full all-ones sum, and the other products must cancel exactly for arbitrary data. That forces 8 rotations per independent output, so 24 rotations is the floor.
- **Halving tricks don't work.** Computing the sums of `p0 + p1` and `p0 - p1` would need a ½ factor to recover the outputs, which is a constant and not allowed.

**What this kernel changes:** it computes `output_2` as the slot sum of `p0 + p1 + p2` minus `output_0` and `output_1`. This keeps 3 multiplications, 3 rescales and 24 rotations, and adds a few cheap additions. The circuit differs from the baseline, but it should be about 1 ms slower.

```cpp
Ciphertext reduce_all(Ciphertext r)
{
  for (int steps = slots() >> 1; steps > 0; steps >>= 1)
    r += r << steps;
  return r;
}

void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");
  Ciphertext p0 = indicator * t0;
  Ciphertext p1 = indicator * t1;
  Ciphertext p2 = indicator * t2;
  Ciphertext o0 = reduce_all(p0);
  Ciphertext o1 = reduce_all(p1);
  Ciphertext s = reduce_all(p0 + p1 + p2);
  Ciphertext o2 = s - o0 - o1;
  o0.set_output("output_0");
  o1.set_output("output_1");
  o2.set_output("output_2");
}
```
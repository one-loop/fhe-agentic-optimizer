The idea is to build the shifted masks directly as plaintexts, `first_slot >> 1` and `first_slot >> 2`. This removes the two ciphertext placement rotations, since each reduced total is uniform across slots and can be masked straight into its output slot. The DSL may not support rotating a plaintext. If this fails to compile, I'll go back to the baseline.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext r0 = indicator * t0;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r0 += r0 << static_cast<int>(s);
  Ciphertext r1 = indicator * t1;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r1 += r1 << static_cast<int>(s);
  Ciphertext r2 = indicator * t2;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
    r2 += r2 << static_cast<int>(s);

  Plaintext m1 = first_slot >> 1;
  Plaintext m2 = first_slot >> 2;

  Ciphertext out = r0 * first_slot;
  out += r1 * m1;
  out += r2 * m2;
  out.set_output("output");
}
```
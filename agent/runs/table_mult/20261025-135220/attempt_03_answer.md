I still can't find a legal way to reduce the circuit. Here is why:

- **No mixing without masks:** without masks, every intermediate value is a plain sum of rotations of `r0`, `r1` and `r2`. Mixing columns would pollute an output, and cancelling that later costs as many rotations as it saves.
- **No shared rotations:** the rotation count is already the minimum, 8 per column (log2 of the slot count) with rotate-and-add.
- **Level cost:** the multiplication already comes first, so all rotations run at the cheaper post-rescale level.

I am resubmitting a variant that uses a different but equivalent algebraic route: it reduces `r0+r1+r2`, `r1`, and `r2` and takes differences. It changes the circuit slightly, with 3 reductions and 2 extra adds or subtractions. It is unlikely to be faster, but it is a valid, different circuit.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  Ciphertext r0 = indicator * columns[0];
  Ciphertext r1 = indicator * columns[1];
  Ciphertext r2 = indicator * columns[2];
  Ciphertext a = r0 + r1 + r2;
  Ciphertext b = r1;
  Ciphertext c = r2;
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    a += a << static_cast<int>(steps);
    b += b << static_cast<int>(steps);
    c += c << static_cast<int>(steps);
  }
  Ciphertext o0 = a - b - c;
  o0.set_output("output_0");
  b.set_output("output_1");
  c.set_output("output_2");
}
```
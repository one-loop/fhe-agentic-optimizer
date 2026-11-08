I still see no way to lower the rotation count without masks or constants. Any packing of two outputs into one ciphertext cannot be separated again. For this attempt I'll try a different structure that changes the circuit: reduce `table_0` and `table_1` directly, then get the third output as the reduction of the sum of all three products minus the first two. It uses more operations, so it probably won't win, but it tests the structure.

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

This is structurally the baseline again, which I should state plainly. I have no verified cheaper circuit, and the baseline remains the best.
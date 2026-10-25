I haven't found a cheaper circuit yet, so this attempt only changes the order of operations and may be rejected as the same circuit.

The obstacle is that the only available mask is `first_slot`, so three separate reductions are needed. After a reduction each total is uniform across slots, so combining the totals needs three masks (`first_slot`-multiplications). I tried several ways to merge the masks or share a reduction and none stayed exact.

This kernel mask-multiplies the products' reductions in a different order, with the shift applied before the final add of each term.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Ciphertext out;
  for (int i = 2; i >= 0; --i)
  {
    Plaintext col("table_" + to_string(i));
    Ciphertext x = indicator * col;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    Ciphertext m = x * first_slot;
    if (i == 2)
      out = m;
    else
      out = (out >> 1) + m;
  }
  out.set_output("output");
}
```
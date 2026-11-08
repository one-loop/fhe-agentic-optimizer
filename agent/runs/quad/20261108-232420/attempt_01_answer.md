The baseline is already minimal: one multiplication at one level, with no rotations. The only remaining lever is the multiplication itself. `square(x)` maps to SEAL's dedicated squaring routine, which is slightly cheaper than a general ciphertext-ciphertext multiply because it reuses the shared operand terms. It also counts as a different circuit (square=1 instead of ct_ct_mul=1), so it is eligible to be judged against the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```
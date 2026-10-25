I still have no cheaper circuit. One ciphertext-ciphertext multiply is the minimum for `x0 * x1`, and the baseline already uses exactly one. I'm submitting it unchanged for the final attempt, so the baseline at 64.30 ms stays the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
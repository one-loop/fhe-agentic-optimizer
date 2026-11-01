This kernel is already at the minimum: one ciphertext-plaintext multiply, which is the only operation `y = x0 * p` needs. The 7.03 to 7.05 ms spread is measurement noise, not a kernel difference. I'm stopping here and resubmitting the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
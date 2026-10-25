Attempt 4 measured 0.80 ms. The four runs of this identical program (1.71, 0.76, 0.74, 0.80 ms) against the 0.72 ms baseline show run-to-run noise, not a real difference. The kernel is one ciphertext add, which is the minimum for x0 + x1, so I'm leaving it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
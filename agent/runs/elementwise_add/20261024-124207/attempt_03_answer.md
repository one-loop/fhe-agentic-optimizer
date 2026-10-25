Attempt 2 produced the same program again: one ciphertext add, with 0.76 ms measured against the 0.72 ms baseline. The spread across the runs (0.72, 1.71, 0.76 ms) comes from timing noise, not from the kernel. The add is the only operation required, so I see no way to lower the operation count. I'm submitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
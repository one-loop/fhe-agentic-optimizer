Another identical program, now at 73.66 ms. Across six runs the times range from 64.87 to 73.66 ms with no change in the code, so this is measurement noise. The kernel has the minimum work for the function: one ciphertext-ciphertext multiply with its relinearization and rescale. I have no formulation that removes any of these, so I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
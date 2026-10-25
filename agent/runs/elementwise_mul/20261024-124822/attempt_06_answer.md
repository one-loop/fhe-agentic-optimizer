This run came in at 71.50 ms with the identical program, which confirms the spread across attempts (64.87 to 71.50 ms) is measurement noise. The kernel still has one ciphertext-ciphertext multiply with its relinearization and rescale, and I have no valid formulation that removes any of them. Any alternative would add operations, not remove them. I'm submitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
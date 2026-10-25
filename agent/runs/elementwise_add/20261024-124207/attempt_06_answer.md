Attempt 5 measured 0.75 ms, which is again within noise of the 0.72 ms baseline. The program is already a single ciphertext add with no multiplications, rotations, or level changes, so there is nothing left to remove. I'm submitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
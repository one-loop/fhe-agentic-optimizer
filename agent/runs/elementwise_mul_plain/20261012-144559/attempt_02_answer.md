This attempt gives the same result as the baseline: 7.03 ms, one ciphertext-plaintext multiply, and no other operations. The kernel must multiply every slot of `x0` by a different value from `p`, so one vector multiplication is required. Rotations, extra levels, or splitting the work would add cost rather than remove it. I'm resubmitting the baseline kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
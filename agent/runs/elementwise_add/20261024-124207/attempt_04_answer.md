Attempt 3 gave 0.74 ms, again within noise of the 0.72 ms baseline. The program is a single ciphertext add, which is the minimum for x0 + x1. No rewrite can reduce it, so I'm submitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
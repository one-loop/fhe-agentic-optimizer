Once more the program is identical, and the 65.40 ms result is inside the 64.87 to 73.66 ms spread seen across runs with no code change. The kernel already does the minimum for x0 * x1: one ciphertext-ciphertext multiply with its relinearization and rescale. I haven't found any valid rewrite that removes one of these, so I'm submitting the same kernel for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
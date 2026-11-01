This run measured 2.49 ms, a new best, but the kernel is unchanged: one plaintext add, with no multiplications, rotations, or level use. The differences between runs are noise. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
This attempt ran at 2.64 ms, slower than the best (2.51 ms), but the computation is identical: one plaintext add, with no multiplications, rotations, or level use. The variation across runs is noise, and nothing in the kernel can reduce the cost of encoding `p` at runtime. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
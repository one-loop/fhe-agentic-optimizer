This run came in at 2.51 ms, the best so far. The computation is unchanged: one plaintext add with no multiplications, rotations, or level use, so the spread between runs is timing noise. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
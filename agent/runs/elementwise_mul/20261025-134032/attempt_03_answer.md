I still have no cheaper circuit. One ciphertext-ciphertext multiply (with relinearization and rescale) is the minimum for x0 · x1 at depth 1, and every algebraic rewrite I can find adds multiplications, rotations or plaintext constants. So I'm submitting the same kernel again. It can't be ranked as a new best, but it is correct.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
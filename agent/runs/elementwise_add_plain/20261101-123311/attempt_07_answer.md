I'm resubmitting the same kernel, because the baseline is already the cheapest possible circuit. It is a single plaintext addition with no other operations, which is the minimum needed to compute `x0 + p`. Any different circuit adds work and would be slower, so the remaining attempts can't produce a win.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
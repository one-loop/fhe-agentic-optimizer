This task is a single ciphertext-plaintext multiply, which is already the minimum for computing x0 * p. The only distinct circuits I can think of would add operations, so they would be slower.

One alternative is to compute the product through a different structure, for example (x0 + x0) * (p/2). That is not possible, because it needs a constant and extra plaintext arithmetic, and it adds operations anyway. Any rewrite either gets normalized back to the same circuit or costs more.

I'm resubmitting the baseline kernel unchanged. It uses one ct_pt_mul and one rescale, so I don't expect it to beat the best, but nothing cheaper is available.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
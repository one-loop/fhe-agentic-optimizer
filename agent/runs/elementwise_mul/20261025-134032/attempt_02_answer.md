I don't have a cheaper circuit for this one. The kernel is a single ciphertext-ciphertext multiply with its relinearization and rescale, and I can't remove any of those.

The only alternative is the square-based identity 2·x0·x1 = (x0+x1)² − x0² − x1². It has two problems:
- **Cost:** it needs three squares, which is more expensive than one multiply.
- **Constants:** it gives 2·x0·x1, and halving that needs a numeric constant, which CKKS rules forbid here.

I'm resubmitting the baseline kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
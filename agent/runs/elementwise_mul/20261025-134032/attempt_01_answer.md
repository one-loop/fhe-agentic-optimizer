This task is a single ciphertext-ciphertext multiply, and I don't see a cheaper way to compute it. The multiply, its relinearization and its rescale are the minimum for x0 * x1 under CKKS.

One alternative is the identity x0*x1 = ((x0+x1)² − (x0−x1)²)/4. That needs two squares and a scalar, so it is strictly worse. Another is `square` instead of a multiply, but that only applies when both operands are the same value, which they aren't here.

So I'm resubmitting the baseline kernel. It will probably not register as a new best, since its circuit is identical to the baseline's.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
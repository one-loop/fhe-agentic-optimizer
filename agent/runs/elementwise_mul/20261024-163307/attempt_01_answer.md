The task is a single ciphertext-ciphertext multiply, which is already the minimum for x0*x1. There is no cheaper way to compute a product of two independent encrypted inputs.

One idea is to use the identity x0*x1 = ((x0+x1)^2 - (x0-x1)^2)/4. That needs squares and a constant of 1/4, which can't be written without a plaintext input. It would also add work, so it can't beat one multiply.

The best I can do is submit a kernel with the same circuit as the baseline. It won't count as new, but it is correct and has the lowest achievable cost.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
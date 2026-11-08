The answer is the same: one ciphertext-plaintext addition is the minimum circuit for `y = x0 + p`. The only operation is one add, with 0 levels, 0 multiplications, 0 rotations and 0 rescales. No correct circuit has fewer operations, so no kernel can be confirmed faster than the baseline.

I am submitting the optimal kernel again rather than a deliberately worse one.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
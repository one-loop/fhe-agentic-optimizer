The baseline is already a single ciphertext add, which is the minimum for this function. Any other circuit would only add operations, so I can't find a faster one. I'm returning the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
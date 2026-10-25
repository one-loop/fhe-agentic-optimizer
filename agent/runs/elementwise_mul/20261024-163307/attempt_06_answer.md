The baseline is still the best I have. One ciphertext-ciphertext multiply is the minimum for `x0 * x1`, and every alternative needs more operations or constants CKKS can't express. I'm returning the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
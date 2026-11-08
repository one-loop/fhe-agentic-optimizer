A single ciphertext-ciphertext multiply is already the minimum for this function. The alternatives I considered are no cheaper:

- **Square identity:** `x0*x1 = ((x0+x1)² − (x0−x1)²)/4` needs two squares plus a constant 1/4 plaintext multiply and an extra level.
- **Rotations or plaintext tricks:** these add operations and don't remove the multiply, relinearization or rescale.

I don't see a way to lower the operation count, so I'm resubmitting the baseline circuit. It is unlikely to be confirmed as faster.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```
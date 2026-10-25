The kernel is still one ciphertext-plaintext multiply plus its rescale, which is the minimum for `x0 * p`. The operation counts match the earlier attempts, so the 13.03 ms result is within run-to-run noise (about 13.0 to 13.5 ms). I don't see a valid rewrite that removes work, so I am keeping the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
The kernel is still one ciphertext-plaintext multiply plus its rescale, which is the minimum work for `x0 * p`. Attempts 1 and 2 had identical operation counts, and the 13.06 ms vs 13.15 ms difference is run-to-run noise. I see no way to reduce the work further, so I am keeping the best kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
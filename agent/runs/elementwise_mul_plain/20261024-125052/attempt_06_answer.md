The kernel is still a single ciphertext-plaintext multiply with its rescale, which is the minimum work for `x0 * p`. Attempts 1 to 5 all had identical operation counts, and their times (13.03 to 13.38 ms) differ only by run-to-run noise. I have no valid rewrite that removes work, so I am keeping the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
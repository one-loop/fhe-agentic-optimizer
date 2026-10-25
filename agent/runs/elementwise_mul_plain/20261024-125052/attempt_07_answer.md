The kernel is still a single ciphertext-plaintext multiply with its rescale, which is the minimum work for `x0 * p`. Attempts 1 to 6 all had identical operation counts, and the times (13.03 to 13.73 ms) vary only from run-to-run noise. I have no valid rewrite that removes work, so I am keeping the best kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
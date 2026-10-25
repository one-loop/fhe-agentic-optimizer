The kernel is still a single ciphertext-plaintext multiply with its rescale, which is the minimum work for `x0 * p`. All seven attempts had identical operation counts, and their times (13.03 to 13.73 ms) vary only from run-to-run noise. I have no valid rewrite that removes work, so I am submitting the same kernel for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
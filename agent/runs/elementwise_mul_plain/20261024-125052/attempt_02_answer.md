The kernel is still a single ciphertext-plaintext multiply with its rescale, which is the minimum for `x0 * p`. The 13.06 ms vs 13.52 ms difference is measurement noise, since the operation counts are identical. Any extra operation would only add cost, so I am keeping this kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
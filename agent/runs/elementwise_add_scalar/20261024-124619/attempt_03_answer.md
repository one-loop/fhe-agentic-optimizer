The kernel is still a single plaintext add, which is the minimum for this function. The timing differences between attempts (0.55, 0.54, 0.38 ms) come from run-to-run noise, since the generated program is identical each time. No other formulation can do less work, so I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
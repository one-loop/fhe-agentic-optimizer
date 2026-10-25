The kernel is still a single plaintext add, the minimum for this function. The generated program is identical each time, so the 0.38–0.55 ms spread across attempts is run-to-run noise. I have no change that reduces work, so I'm submitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
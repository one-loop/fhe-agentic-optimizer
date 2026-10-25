The kernel is still a single plaintext add, which is the minimum for this function. The generated program is identical on every attempt, so the 0.38–0.55 ms spread is run-to-run noise. I'm submitting the same kernel for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
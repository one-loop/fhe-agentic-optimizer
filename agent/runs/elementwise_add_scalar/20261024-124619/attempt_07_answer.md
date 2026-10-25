The kernel is still a single plaintext add, so no operation can be removed. The remaining 0.38–0.55 ms spread comes from run-to-run noise, since the generated program is identical on every attempt. I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
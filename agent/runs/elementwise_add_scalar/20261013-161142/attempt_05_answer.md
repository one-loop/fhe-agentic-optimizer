The kernel is still one ciphertext-plaintext add, the minimum possible. Attempt 4 measured 0.43 ms, within the 0.39–0.51 ms range seen across identical kernels, so the spread is timing noise rather than a code difference. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
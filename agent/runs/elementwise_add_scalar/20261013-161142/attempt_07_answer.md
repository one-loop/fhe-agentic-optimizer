The kernel is one ciphertext-plaintext add, the minimum possible, and attempt 6 measured 0.41 ms, within the same noise band as the other runs. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
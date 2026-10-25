The kernel is still a single ciphertext-plaintext add, which is the minimum possible. Attempt 3 measured 0.39 ms against 0.45 ms before, but the computation is unchanged, so that gap is timing noise. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
I'm resubmitting the same kernel. It is one ciphertext-plaintext add, with no multiplications, rotations, or level consumption, so nothing cheaper exists. Attempt 7 measured 0.43 ms, and the spread across identical runs (0.39–0.62 ms) is timing noise. Attempt 3's 0.39 ms is the best recorded result, and this code is identical to it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
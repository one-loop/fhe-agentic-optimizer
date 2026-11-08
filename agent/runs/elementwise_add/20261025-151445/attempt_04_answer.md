The kernel is still a single ciphertext add, and nothing cheaper exists. The output depends on both inputs, so at least one operation must combine them. All other operation counts are already zero, so there is nothing left to remove.

I'm not going to submit padded or contrived variants. They would only add work and run slower. Resubmitting the same kernel will keep being judged identical to the baseline, so the baseline at 0.75 ms stands as the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
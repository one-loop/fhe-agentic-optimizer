I can't find a circuit cheaper than the baseline. Attempt 3 confirmed that extra multiplications double the time (129.69 ms against 64.23 ms). A single ct-ct multiply with its relinearization and rescale is the minimum for x², because there are no plaintext inputs, rotations or other terms to trade away. I'm keeping the best kernel, which is the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```
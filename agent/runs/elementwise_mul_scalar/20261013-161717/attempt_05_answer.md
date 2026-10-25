I'm keeping the kernel as is. It performs one ciphertext-plaintext multiply and one rescale, which is the minimum for `y = x0 * s`. Across five runs the time stays between 8.92 and 9.34 ms, so the spread is measurement noise, not something a different structure would fix.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
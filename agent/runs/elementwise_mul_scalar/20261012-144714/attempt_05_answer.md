Attempt 4 measured 4.58 ms, a difference of about 0.01 ms from the baseline. That is within run-to-run noise, not a real gain. The kernel is unchanged: one ciphertext-plaintext multiply and one rescale, which is the minimum for x0 * s at depth 1.

Best kernel (4.58–4.59 ms):

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
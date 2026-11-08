No cheaper circuit exists, so I'm submitting the baseline again. It uses one ciphertext × plaintext multiply, the rescale CHEHAB adds to it, and one level, which is the minimum for `y = x0 * p` with an arbitrary dense `p`. Every structurally different kernel adds operations and would be slower. The best result for this task is the baseline at 2.89 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
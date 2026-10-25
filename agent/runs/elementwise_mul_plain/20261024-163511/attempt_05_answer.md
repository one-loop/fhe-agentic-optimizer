I'm still returning the baseline kernel. I haven't found any circuit for `y = x0 * p` with fewer operations than one ct-pt multiply plus its rescale, and the last three attempts confirmed that resubmitting it can't be judged faster.

I could spend the remaining attempts on circuits with extra operations, but they would only be slower, so I'm not going to. The baseline stays the best result at 12.92 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
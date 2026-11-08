Nothing I can build has fewer operations than the baseline's single add. The output depends on both inputs, so it needs at least one operation. A different circuit can only add operations (attempt 3 measured 1.47 ms), or break correctness (attempt 2 failed with a transparent ciphertext). Resubmitting the baseline again would just be a repeat that cannot win, so I'm not going to spend more attempts on variants I expect to be slower. The baseline stays the best kernel, and I'm submitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
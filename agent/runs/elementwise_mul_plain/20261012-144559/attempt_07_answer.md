Attempt 6 matches the baseline again: correct, 7.04 ms, one ciphertext-plaintext multiply. This kernel is already minimal for `y = x0 * p`, so I'm submitting it unchanged as the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
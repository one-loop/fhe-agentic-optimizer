The kernel is unchanged: one ciphertext-plaintext multiply with its rescale. Timings across identical submissions (8.80, 8.89, 9.46, 9.01, 8.80 ms) are noise. I see no cheaper way to multiply by an arbitrary plaintext scalar in this DSL, so I'm resubmitting it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
I have no cheaper circuit for the last attempt either. The baseline is already minimal: one plaintext multiply with its rescale, plus one plaintext add. Everything I have tried was rejected, slower, or normalized to the baseline circuit. Attempts 1, 4, 6 and 7 were the baseline circuit again, attempt 5 repeated attempt 3, and attempt 3 timed 3.95 ms against 3.77 ms.

I'm keeping the baseline as the best kernel and returning it unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```
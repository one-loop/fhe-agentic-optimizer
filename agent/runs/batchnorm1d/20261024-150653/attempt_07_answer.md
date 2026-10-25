I have nothing new to try. Attempt 6 was the baseline circuit again, and I can't find a way to compute `x*A + B` with fewer than one ct-pt multiply (with its rescale) and one plaintext add. Every alternative I've checked either isn't allowed (plaintext division, constants, plaintext-plaintext arithmetic) or adds operations.

I'm not going to spend the last attempts on more rewrites that CHEHAB reduces to the same circuit. The baseline kernel at 17.95 ms is my best answer. Re-timing it can only add noise.

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
I still have no cheaper circuit for `y = x*A + B`. The baseline already has the minimum of one plaintext multiply with its rescale and one plaintext add. Every rewrite I can build either adds operations (attempt 3 was slower at 3.95 ms against 3.77 ms), is rejected for plaintext-plaintext arithmetic (attempt 2), or normalizes to a circuit I have already tried (attempts 1, 4, 5 and 6).

I won't spend the last two attempts on more duplicate variants. The baseline stays the best kernel, and I'm returning it unchanged.

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
I don't see a cheaper circuit. The baseline already has the minimum: one plaintext multiply with its rescale, and one add. Every alternative I can write legally adds operations, as attempt 4 did, or needs forbidden plaintext-plaintext arithmetic. I'm submitting the baseline form again.

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
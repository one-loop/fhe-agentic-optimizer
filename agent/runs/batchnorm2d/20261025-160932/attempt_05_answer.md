I haven't found a circuit cheaper than the baseline. It already has the minimum: one plaintext multiply with its rescale, and one plaintext add. Every alternative I tried either normalized to the same circuit or added operations. I'm resubmitting the baseline form.

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
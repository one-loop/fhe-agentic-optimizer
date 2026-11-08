I haven't found a legal circuit with fewer operations than the baseline. It needs one plaintext multiply, one rescale and one plaintext add. Every alternative I tried either collapsed to the same circuit or added negations, which was slower. For the last attempt I'm keeping the baseline form.

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
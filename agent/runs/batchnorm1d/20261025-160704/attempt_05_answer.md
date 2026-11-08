I still see no cheaper exact circuit. CHEHAB folded the x - x trick away in the last attempt. The baseline's one ct-pt multiply, one rescale and one plaintext add look like the minimum for this function. I'm resubmitting that form.

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
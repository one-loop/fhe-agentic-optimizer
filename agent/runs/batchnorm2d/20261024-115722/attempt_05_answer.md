The kernel is still at its minimum: one plaintext multiply with rescale and one plaintext add. The 18.07 to 18.54 ms spread across attempts is measurement noise, because I have been submitting the identical program each time. I can't find a variant that removes an operation without changing the function or using disallowed plaintext-plaintext arithmetic, so I'm keeping this form.

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
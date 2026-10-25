I've submitted the same program six times, so the 18.07 to 18.54 ms spread is only noise. Nothing in the results shows an improvement, and I shouldn't claim one.

The kernel needs one plaintext multiply by A, one rescale and one plaintext add of B. I can't remove any of these without changing the function. Folding B into the multiply would need plaintext-plaintext arithmetic, which the rules forbid. Trying other variants would only add operations, so I'm keeping the minimal form.

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
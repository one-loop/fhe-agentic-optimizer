I could not find a cheaper circuit. `y = x*A + B` needs one plaintext multiply with its rescale, plus one plaintext add. Attempt 3's negation variant added operations and measured within noise of the baseline (about 0.98x, not confirmed faster). Every other rewrite either needs plaintext-plaintext arithmetic, which is not allowed, or adds work. I'm returning the baseline form as the best kernel.

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
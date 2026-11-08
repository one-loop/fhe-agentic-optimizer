I still have no cheaper circuit. Every form I tried either normalizes to the baseline circuit or adds operations (attempt 2 added two adds and was slower at 19.31 ms).

The floor is one plaintext multiply with its rescale, plus one plaintext add. Folding B into the multiply would need B/A or a ciphertext of ones, and CKKS rules 1 and 2 forbid both. The baseline form is therefore the best available, and I'm resubmitting it unchanged.

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
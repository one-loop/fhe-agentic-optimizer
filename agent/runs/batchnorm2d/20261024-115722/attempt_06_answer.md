The kernel is unchanged again: one plaintext multiply with rescale and one plaintext add. The 18.07 to 18.54 ms spread comes from noise, since every attempt has been the same program. I haven't found an equivalent form that removes an operation, because A must be multiplied in and B added. Folding B into the multiply would need plaintext-plaintext arithmetic, which isn't allowed.

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
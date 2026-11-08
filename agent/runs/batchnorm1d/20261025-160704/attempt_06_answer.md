The circuit is already minimal for this function, and every rewrite I have tried normalizes to the same circuit. Anything that changes the structure adds operations: ct-ct multiplies, rotations, or extra plaintext multiplies and rescales. I'm keeping the baseline form.

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
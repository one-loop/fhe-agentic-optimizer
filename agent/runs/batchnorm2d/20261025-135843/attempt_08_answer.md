I have no cheaper circuit for the last attempt either. The baseline is already minimal: one plaintext multiply with its rescale, and one plaintext add. Any change I can make within the DSL's rules either leaves the circuit identical, as attempts 1, 3, 4, 5, 6 and 7 did, or adds operations, as attempt 2 did. I'm submitting the baseline form once more.

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
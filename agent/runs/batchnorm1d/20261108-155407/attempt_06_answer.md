I have no structurally different circuit that could be cheaper. The baseline's single plaintext multiply with its rescale, plus one plaintext add, is the floor for `y = x*A + B` under these rules. Every alternative I can build adds operations, such as negations, extra additions or zero ciphertexts, and attempt 3 showed those land within noise. I'm keeping the baseline form as my answer.

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
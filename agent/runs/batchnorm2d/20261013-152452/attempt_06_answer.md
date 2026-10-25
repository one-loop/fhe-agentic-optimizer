I'm keeping the same kernel. Attempt 5 measured 18.07 ms with the same operation counts as the earlier runs, so the spread across attempts is timing noise. The program is already the minimum for `y = x * A + B`: one ciphertext-plaintext multiply, one rescale, one plaintext add, and no rotations or ciphertext-ciphertext multiplies.

```cpp
// y = x * A + B. Minimal form: one ct-pt multiply (one level) plus one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```
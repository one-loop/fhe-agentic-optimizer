I'm keeping the same kernel. Attempt 6 measured 18.26 ms with the same operation counts as every prior run, so the differences across attempts are noise. The computation is already minimal for `y = x * A + B`: one ciphertext-plaintext multiply with one rescale, and one plaintext add. There are no rotations or ciphertext-ciphertext multiplies to remove.

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
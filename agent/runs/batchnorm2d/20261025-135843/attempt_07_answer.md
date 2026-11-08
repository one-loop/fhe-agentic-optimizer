I can't find a cheaper circuit, and I don't want to submit a worse one just to make it look different. The baseline has the minimum operation set: one plaintext multiply, one rescale, one plaintext add, with no rotations or ciphertext-ciphertext multiplies. The only ways to remove any of these are a ciphertext of ones or plaintext-plaintext arithmetic (for example `B/A` or `A*B`), and the DSL forbids both. I'm keeping the baseline form.

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
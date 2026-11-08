I'm out of structurally different, cheaper options. The baseline circuit has the minimum operation set for this function: one plaintext multiply with its rescale, and one plaintext add. Everything I've tried is either normalized back to that circuit or adds operations. Every other rewrite needs plaintext division, plaintext-plaintext arithmetic, or numeric constants, none of which CKKS in this DSL allows.

I'm submitting the same optimal form again.

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
The baseline is already the minimum circuit. The output depends on both inputs, so at least one operation is needed. That operation is a single ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments. Every alternative I can build (negations, double adds, multiplying by a one-valued plaintext) adds operations and cannot be cheaper. I am therefore keeping this form, and any timing difference would be noise.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
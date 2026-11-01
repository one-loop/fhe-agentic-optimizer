The baseline already does the minimum possible work: one plaintext-vector add, with no multiplications, rotations, or level consumption. Any alternative formulation would add operations, so I'm keeping the same computation. The remaining cost is the plaintext encoding inside the add, which the kernel cannot avoid because `p` is a runtime input.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
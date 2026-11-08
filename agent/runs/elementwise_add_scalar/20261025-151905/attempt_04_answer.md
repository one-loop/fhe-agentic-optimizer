The baseline is already minimal, so I have no cheaper circuit to propose. The output must depend on both inputs, which needs at least one ciphertext-plaintext add. That is the whole baseline circuit, with no multiplications, rotations, rescales or alignments. Any structurally different kernel would add operations and could not beat it, so I'm keeping the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
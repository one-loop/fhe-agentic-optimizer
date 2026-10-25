I have no cheaper circuit to propose, so I'm resubmitting the baseline kernel unchanged. It is a single ciphertext-plaintext add, with no multiplications, rotations, rescales or alignments to remove. The remaining cost is encoding `p` and the add itself, and neither can be avoided in the CKKS DSL. Any structurally different circuit would only add operations and be slower. Further attempts would just repeat this.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
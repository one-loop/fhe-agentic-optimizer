A single ciphertext add is already the minimum: no multiplications, rotations, rescales or alignments. The output depends on both inputs, so at least one binary operation is required. Any structurally different circuit would only add operations and be slower.

The only knob left is how the add is expressed, and CHEHAB normalizes that away. I'm submitting the same minimal kernel again, since the baseline is already optimal for this function.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```
The baseline is still the best possible kernel for this task. It is a single plaintext addition with no multiplications, rotations, rescales or level use. Any circuit that computes `x0 + p` and differs from it has to add operations, so it would be slower and could not pass the 3%-faster check.

The remaining attempts will not produce a faster correct kernel, so I am submitting the optimal one again. Feel free to stop the loop here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
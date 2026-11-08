This task has no cheaper circuit than the baseline. It uses only what the function strictly requires: one ciphertext × plaintext multiply and its rescale, at one level, with no rotations, ct-ct multiplies or alignments. Any structurally different kernel would add operations and be slower. Resubmitting would only produce another identical, untimed result, so I'm stopping the search here.

The final, optimal kernel is the baseline itself:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
I still have no cheaper circuit. Resubmitting the same kernel would only repeat attempts 1 and 2, so the baseline stays the best. `y = x0 * s` with a run-time `s` needs at least one ciphertext-plaintext multiplication and its rescale. That is the whole baseline circuit: no ciphertext multiplications, no rotations, no alignments, and one level. Any change I can make only adds operations.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
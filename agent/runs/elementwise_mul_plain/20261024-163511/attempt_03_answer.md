I have no new circuit to propose. The baseline's single ct-pt multiply plus its rescale is the minimum for `y = x0 * p`. Any structural change I can build (extra adds, mod switches, rotations, or a ct-ct multiply) adds operations and would be slower. Resubmitting the same kernel can't be judged faster, but it is correct.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
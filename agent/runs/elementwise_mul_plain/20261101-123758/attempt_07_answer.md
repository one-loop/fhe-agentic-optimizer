I'm resubmitting the baseline, which is the minimal circuit. It is one ct-pt multiply plus CHEHAB's automatic rescale. Nothing cheaper can compute `x0 * p` when `p` is a plaintext known only at run time. Any structural change would only add operations.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
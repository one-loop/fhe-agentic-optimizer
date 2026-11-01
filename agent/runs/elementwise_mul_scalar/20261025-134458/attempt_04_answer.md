I have no cheaper circuit to propose, so I'm resubmitting the baseline form. The baseline is one scalar ct-pt multiply plus one rescale, and that already looks like the minimum. Every alternative I can build needs more multiplies, rescales or plaintext adds. For example, `(x0+x0)*s - x0*s` in attempt 2 doubled the multiplies and rescales and took 9.64 ms. Resubmitting this form can't register as an improvement, but I'd rather not submit something I know is slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
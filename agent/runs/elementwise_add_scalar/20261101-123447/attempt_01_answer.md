The baseline is already minimal and I don't expect any kernel to beat it. It is a single ciphertext-plaintext addition: no multiplications, rotations, rescales or level use, at 0.26 ms. The output `x0 + s` needs at least one operation that combines the ciphertext with the plaintext, so no correct circuit can use fewer operations. Any different circuit would add work and run slower.

Resubmitting the baseline cannot become the new best, since its circuit matches the baseline's. I am returning it anyway, because spending attempts on deliberately slower variants serves no purpose.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
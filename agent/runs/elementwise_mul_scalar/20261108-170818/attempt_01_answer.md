The baseline is already at the minimum. `s` is only known at run time, so the product needs one ciphertext-plaintext multiplication, and that multiplication needs one rescale. Any other formulation (extra adds, doubling, rotations) would add operations. I don't see a cheaper circuit, so this kernel is the same as the baseline and is unlikely to be confirmed faster.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
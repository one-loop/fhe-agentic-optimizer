I'm not going to propose a changed kernel. The baseline is already minimal: one ciphertext-plaintext multiply and one rescale, with no rotations, ciphertext-ciphertext multiplies, or extra adds. Results across attempts are 4.59–4.60 ms, which is run-to-run noise, so no rewrite can give a meaningful speedup.

Final kernel (same as baseline, best measured at 4.59 ms):

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
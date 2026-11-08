The baseline is already minimal: one plaintext add, no multiplications, rotations, rescales or levels. Any rewrite would either add operations or normalize to the same circuit, so I don't expect this kernel to beat the baseline. I'm resubmitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
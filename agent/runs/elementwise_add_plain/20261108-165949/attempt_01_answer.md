The baseline is already a single plaintext-vector add with no multiplications, rotations, rescales or levels. Any rewrite would add operations or reduce to the same circuit, so I'm keeping the minimal form.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```
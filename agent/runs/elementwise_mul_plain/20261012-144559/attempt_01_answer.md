The baseline already performs the minimum possible work: one ciphertext-plaintext multiplication, which is the only operation the output `y = x0 * p` requires. It uses one level, no rotations, and no ciphertext-ciphertext multiplications. Any rewrite would either add operations or leave the count unchanged, so I'm submitting the same kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
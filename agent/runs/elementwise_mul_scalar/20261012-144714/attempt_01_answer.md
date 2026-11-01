The baseline already uses the minimum possible work: one ciphertext-plaintext multiply with the scalar plaintext, no rotations, no ciphertext-ciphertext multiplies, and depth 1. Any rewrite would add operations, such as extra adds or multiplies, so I'm submitting the same kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
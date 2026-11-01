I'm keeping the same kernel. Attempt 7 measured 4.58 ms, the same as attempt 4, and every run has landed between 4.58 and 4.60 ms. That spread is noise. The kernel is already minimal: one ciphertext-plaintext multiply and one rescale, with no rotations, no ciphertext-ciphertext multiplies, and depth 1.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
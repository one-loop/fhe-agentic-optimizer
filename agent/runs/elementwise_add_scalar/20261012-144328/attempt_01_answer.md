The baseline already does the minimum possible work: one ciphertext-plaintext addition, with no multiplications, rotations, or level consumption. There is no cheaper way to compute x0 + s under these rules, so I'm returning the same kernel unchanged. Any rewrite would add operations rather than remove them.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```
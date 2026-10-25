The baseline is already a single ciphertext-plaintext multiply plus its rescale, which is the minimum work for `x0 * p`. There is no rotation, no ciphertext-ciphertext multiply, and no add to remove, and any rewrite would only add operations. I am resubmitting the same kernel, so I expect no speedup. Any timing difference would be run-to-run noise.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```
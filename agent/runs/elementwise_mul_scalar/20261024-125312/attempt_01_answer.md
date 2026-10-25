The baseline is already a single ciphertext-plaintext multiply plus its rescale, which is the minimum for y = x0 * s. Any rewrite (for example doubling via additions) can't produce an arbitrary scalar without a multiplication. I'm submitting the same kernel, since I see no valid way to remove the multiply.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```
I still don't have a cheaper circuit. Every two-multiplication form needs the same level alignment of `second`, and the identities I tried (`first + (second - first)*keep`, `(first - second)*drop + second`) all produce the same operation counts as the best. The three-multiplication form with no alignment was measured slower in attempt 2. Rotating before masking is not possible with only the `keep_front` and `drop_front` masks. I'm keeping the best kernel.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = (second - t) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```
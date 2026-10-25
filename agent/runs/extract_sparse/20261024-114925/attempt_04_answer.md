I'm out of structural savings. Every variant I tried that cuts a multiply costs an extra rotation (~20 ms) or an extra alignment. The remaining form has the minimum for this mask set:
- 2 plaintext multiplies
- 2 rotations, one per output ciphertext
- no multiply-free way to build either output

I'm resubmitting the best kernel, attempt 1's form, which uses the subtraction `second - t` for `u`. Attempts 1 and 2 ran the same code and differed only by timing noise.

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
  Ciphertext u = second - t;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = u << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```
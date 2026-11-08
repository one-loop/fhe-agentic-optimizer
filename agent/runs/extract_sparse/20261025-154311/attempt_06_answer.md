I haven't found a circuit that beats the best, and I don't want to submit another duplicate. Two plaintext multiplications and two rotations look like the practical minimum with only the `keep_front` and `drop_front` masks. Going lower would need a rotated copy of a mask, and the DSL doesn't allow plaintext arithmetic to build one. The one remaining alignment is also cheaper than the plaintext multiplication that would replace it. I'm keeping the best kernel (attempt 1, 128.63 ms vs 177.80 ms for the baseline).

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
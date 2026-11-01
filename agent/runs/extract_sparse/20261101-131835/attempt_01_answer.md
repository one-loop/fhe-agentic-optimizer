Since rotation is linear, both `output_0` terms can be added before one shared rotation. I also use `keep_front = 1 - drop_front`: `second * keep_front` equals `second - second * drop_front`, and that product is already needed for `output_1`. This gives `output_0 = (first*drop - second*drop + second) << num_dense`. That needs 2 plaintext multiplies and 2 rotations, against 3 and 3 in the baseline. CHEHAB must align the level-0 `second` with the level-1 terms. Its estimated cost is a mod switch (~1 ms) or a multiply_plain plus rescale (~5 ms), less than the multiply and rotation removed.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t1 = second * drop_front;
  Ciphertext t0 = first * drop_front;
  Ciphertext out0 = ((t0 - t1) + second) << num_dense;
  Ciphertext out1 = t1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```
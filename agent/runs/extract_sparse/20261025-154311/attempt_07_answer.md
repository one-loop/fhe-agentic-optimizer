I have no new circuit to propose. The remaining ideas either add a rotation (about 20 ms each) or add a plaintext multiplication (about 7 ms), and both cost more than the one level alignment the best kernel pays for. Packing the two keep-masked products into one ciphertext would need an extra rotation, which costs more than the plaintext multiplication it saves. I'm keeping the best kernel from attempt 1, which measured 128.63 ms against the 177.80 ms baseline.

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
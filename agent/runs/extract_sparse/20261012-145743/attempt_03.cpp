// out0 = (first*drop + second*keep) << n, out1 = (second*drop) << n.
// Both addends of the sum come from one multiply, so no scale alignment is needed.

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext k = second * keep_front;
  Ciphertext t = second * drop_front;
  Ciphertext out0 = (a + k) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

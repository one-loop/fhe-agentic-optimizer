void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext k = second * keep_front;
  Ciphertext t = first * drop_front;
  Ciphertext out0 = (t + k) << num_dense;
  Ciphertext out1 = (second - k) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

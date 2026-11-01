void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext u = first * drop_front;
  Ciphertext v = second * drop_front;
  Ciphertext out1 = v << num_dense;
  Ciphertext out0 = (u + (second - v)) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

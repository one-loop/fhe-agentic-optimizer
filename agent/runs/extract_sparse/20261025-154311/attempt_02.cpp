void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sd = second * drop_front;
  Ciphertext a = first * drop_front;
  Ciphertext t = second * keep_front;
  Ciphertext out0 = (a + t) << num_dense;
  Ciphertext out1 = sd << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

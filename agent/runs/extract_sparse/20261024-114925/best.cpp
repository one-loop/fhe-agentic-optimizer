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

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext f = first * drop_front;
  Ciphertext k = second * keep_front;
  Ciphertext t = second * drop_front;
  Ciphertext out0 = (f + k) << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

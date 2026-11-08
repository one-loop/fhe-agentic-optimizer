void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext a = first * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext out1 = b << num_dense;
  Ciphertext e = a + second;
  Ciphertext out0 = (e << num_dense) - out1;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

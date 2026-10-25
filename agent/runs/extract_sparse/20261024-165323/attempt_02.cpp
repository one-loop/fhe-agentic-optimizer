void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext s1 = second * drop_front;
  Ciphertext comb = ((first * drop_front) - s1) + second;
  Ciphertext out0 = comb << num_dense;
  Ciphertext out1 = s1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

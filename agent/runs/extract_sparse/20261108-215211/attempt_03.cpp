void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext shifted1 = (second * drop_front) << num_dense;
  Ciphertext merged = (first - second) * drop_front + second;
  Ciphertext out0 = merged << num_dense;
  out0.set_output("output_0");
  shifted1.set_output("output_1");
}

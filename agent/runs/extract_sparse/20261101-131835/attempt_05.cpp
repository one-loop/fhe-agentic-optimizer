void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t0 = first * drop_front;
  Ciphertext t1 = second * drop_front;
  Ciphertext out1 = t1 << num_dense;
  Ciphertext p = (t0 + second) << num_dense;
  Ciphertext out0 = p - out1;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

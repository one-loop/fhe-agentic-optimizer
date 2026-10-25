// Experiment: all mask products are single plaintext multiplies at level 1,
// so the sum needs no level alignment. Rotations: 2 (combined and second*drop).
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);

  Ciphertext F = first * drop_front;     // first*drop
  Ciphertext G = second * keep_front;    // second*keep
  Ciphertext D = second * drop_front;    // second*drop, for output_1

  Ciphertext out0 = (F + G) << num_dense;
  Ciphertext out1 = D << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

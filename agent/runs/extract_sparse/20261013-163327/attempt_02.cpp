// Best so far: rotation is linear, so rotate the combined sum once.
// second*keep = second - second*drop, so only two plaintext multiplies are needed,
// and second*drop is shared between both outputs.
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);

  Ciphertext D = second * drop_front;          // second*drop, shared
  Ciphertext F = first * drop_front;           // first*drop
  Ciphertext combined = (F - D) + second;      // first*drop + second*keep

  Ciphertext out0 = combined << num_dense;
  Ciphertext out1 = D << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}

// Reference for extract_sparse: rotation is linear, so the two rotated parts of
// output_0 share one rotation: (a << k) + (b << k) = (a + b) << k.
//   output_0 = (first * drop_front + second * keep_front) << num_dense
//   output_1 = (second * drop_front) << num_dense           (dense case only)
// It does not use drop_front = 1 - keep_front (a relation between the inputs).
// Expected: 1 level; fixed case 1 rotation, 2 ct x pt; dense case 2 rotations, 3 ct x pt.
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const size_t n = slots();
  const int num_dense = static_cast<int>(n / 4);
  // 8 slots: the benchmark's extract_sparse_boundary case (one output); more: two outputs.
  const size_t logical_size = n <= 8 ? n + n / 4 : n + n / 2;
  const size_t output_count = (logical_size - n / 4 + n - 1) / n;
  Ciphertext output_0 = (first * drop_front + second * keep_front) << num_dense;
  output_0.set_output("output_0");
  if (output_count > 1)
  {
    Ciphertext output_1 = (second * drop_front) << num_dense;
    output_1.set_output("output_1");
  }
}

// Baseline: nn.Conv2d on the scalar BFV path, as benchmarks/nn_conv2d/nn_conv2d.cpp (fhe()).
// WIDTH is the case's image width (4 or 6), set by the harness; the kernel is 3 x 3.
void fhe()
{
  const size_t in_h = WIDTH, in_w = WIDTH;
  const size_t k_h = 3, k_w = 3;
  const size_t out_h = in_h - k_h + 1;
  const size_t out_w = in_w - k_w + 1;

  vector<vector<Ciphertext>> x(in_h, vector<Ciphertext>(in_w));
  for (size_t r = 0; r < in_h; ++r)
    for (size_t c = 0; c < in_w; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(k_h, vector<Ciphertext>(k_w));
  for (size_t r = 0; r < k_h; ++r)
    for (size_t c = 0; c < k_w; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  for (size_t r = 0; r < out_h; ++r)
  {
    for (size_t c = 0; c < out_w; ++c)
    {
      Ciphertext sum = b + (k[0][0] * x[r][c]);
      for (size_t ki = 0; ki < k_h; ++ki)
      {
        for (size_t kj = 0; kj < k_w; ++kj)
        {
          if (ki == 0 && kj == 0)
            continue;
          sum += (k[ki][kj] * x[r + ki][c + kj]);
        }
      }
      sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}

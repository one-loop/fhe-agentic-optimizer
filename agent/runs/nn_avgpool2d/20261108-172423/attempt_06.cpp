void fhe()
{
  const size_t in_h = WIDTH, in_w = WIDTH;
  const size_t stride = 2;
  const size_t out_h = in_h / stride;
  const size_t out_w = in_w / stride;

  vector<vector<Ciphertext>> x(in_h, vector<Ciphertext>(in_w));
  for (size_t r = 0; r < in_h; ++r)
    for (size_t c = 0; c < in_w; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  for (size_t r = 0; r < out_h; ++r)
  {
    for (size_t c = 0; c < out_w; ++c)
    {
      Ciphertext top = x[r * stride][c * stride] + x[r * stride][c * stride + 1];
      Ciphertext bot = x[r * stride + 1][c * stride] + x[r * stride + 1][c * stride + 1];
      Ciphertext patch_sum = top + bot;
      patch_sum.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}

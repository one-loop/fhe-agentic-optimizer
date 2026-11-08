void fhe()
{
  const size_t in_h = WIDTH, in_w = WIDTH;
  const size_t out_h = in_h - 2;
  const size_t out_w = in_w - 2;

  vector<vector<Ciphertext>> x(in_h, vector<Ciphertext>(in_w));
  for (size_t r = 0; r < in_h; ++r)
    for (size_t c = 0; c < in_w; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (size_t r = 0; r < 3; ++r)
    for (size_t c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  vector<Ciphertext> gs(3);
  for (size_t ki = 0; ki < 3; ++ki)
    gs[ki] = k[ki][0] + k[ki][1];

  for (size_t r = 0; r < out_h; ++r)
  {
    for (size_t c = 0; c + 1 < out_w + 1; c += 2)
    {
      // out_w is even (2 or 4)
      Ciphertext s0 = b;
      Ciphertext s1 = b;
      for (size_t ki = 0; ki < 3; ++ki)
      {
        const Ciphertext &d0 = x[r + ki][c];
        const Ciphertext &d1 = x[r + ki][c + 1];
        const Ciphertext &d2 = x[r + ki][c + 2];
        const Ciphertext &d3 = x[r + ki][c + 3];
        Ciphertext p1 = (d0 - d1) * k[ki][0];
        Ciphertext p2 = d1 * gs[ki];
        Ciphertext p3 = (d2 - d1) * k[ki][1];
        s0 += p1 + p2;
        s1 += p2 + p3;
        s0 += d2 * k[ki][2];
        s1 += d3 * k[ki][2];
      }
      s0.set_output("y_" + to_string(r) + "_" + to_string(c));
      s1.set_output("y_" + to_string(r) + "_" + to_string(c + 1));
    }
  }
}

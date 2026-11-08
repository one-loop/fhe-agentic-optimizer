void fhe()
{
  const size_t n = WIDTH;

  vector<Ciphertext> x(n);
  for (size_t j = 0; j < n; ++j)
    x[j] = Ciphertext("x_" + to_string(j));

  vector<vector<Ciphertext>> w(n, vector<Ciphertext>(n));
  vector<Ciphertext> b(n);
  for (size_t i = 0; i < n; ++i)
  {
    b[i] = Ciphertext("b_" + to_string(i));
    for (size_t j = 0; j < n; ++j)
      w[i][j] = Ciphertext("w_" + to_string(i) + "_" + to_string(j));
  }

  size_t pairs = n / 2;
  vector<Ciphertext> xx;
  for (size_t k = 0; k < pairs; ++k)
    xx.push_back(x[2 * k] * x[2 * k + 1]);

  Ciphertext xsum = xx[0];
  for (size_t k = 1; k < pairs; ++k)
    xsum += xx[k];

  for (size_t i = 0; i < n; ++i)
  {
    Ciphertext pos = w[i][0 + 0] + x[1];
    pos = pos * (w[i][1] + x[0]);
    Ciphertext neg = w[i][0] * w[i][1];
    for (size_t k = 1; k < pairs; ++k)
    {
      pos += (w[i][2 * k] + x[2 * k + 1]) * (w[i][2 * k + 1] + x[2 * k]);
      neg += w[i][2 * k] * w[i][2 * k + 1];
    }
    Ciphertext y = (pos + b[i]) - (neg + xsum);
    y.set_output("y_" + to_string(i));
  }
}

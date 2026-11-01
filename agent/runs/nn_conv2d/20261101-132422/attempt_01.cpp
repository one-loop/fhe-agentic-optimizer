void fhe()
{
  const int W = WIDTH;
  const int out = W - 2;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  // memoized x*x products
  vector<vector<Ciphertext>> xxProd;
  vector<pair<int, int>> xxKey;
  auto xx = [&](int r1, int c1, int r2, int c2) -> Ciphertext {
    int a = r1 * W + c1, bb = r2 * W + c2;
    if (a > bb) { int t = a; a = bb; bb = t; }
    for (size_t i = 0; i < xxKey.size(); ++i)
      if (xxKey[i].first == a && xxKey[i].second == bb)
        return xxProd[i][0];
    Ciphertext p = x[a / W][a % W] * x[bb / W][bb % W];
    xxKey.push_back({a, bb});
    xxProd.push_back(vector<Ciphertext>{p});
    return p;
  };

  // memoized k*k products
  vector<vector<Ciphertext>> kkProd;
  vector<pair<int, int>> kkKey;
  auto kk = [&](int i1, int j1, int i2, int j2) -> Ciphertext {
    int a = i1 * 3 + j1, bb = i2 * 3 + j2;
    if (a > bb) { int t = a; a = bb; bb = t; }
    for (size_t i = 0; i < kkKey.size(); ++i)
      if (kkKey[i].first == a && kkKey[i].second == bb)
        return kkProd[i][0];
    Ciphertext p = k[a / 3][a % 3] * k[bb / 3][bb % 3];
    kkKey.push_back({a, bb});
    kkProd.push_back(vector<Ciphertext>{p});
    return p;
  };

  // per (row parity, col parity): b - sum of kernel pair products
  vector<vector<Ciphertext>> bcorr(2, vector<Ciphertext>(2));
  vector<vector<bool>> haveCorr(2, vector<bool>(2, false));

  for (int r = 0; r < out; ++r)
  {
    for (int c = 0; c < out; ++c)
    {
      int rp = r % 2, cp = c % 2;
      // pairs: (i1,j1),(i2,j2) window coords
      vector<vector<int>> pairs;
      int lj;
      for (int i = 0; i < 3; ++i)
      {
        if (cp == 0) pairs.push_back({i, 1, i, 2});
        else pairs.push_back({i, 0, i, 1});
      }
      lj = (cp == 0) ? 0 : 2;
      int si;
      if (rp == 0) { pairs.push_back({1, lj, 2, lj}); si = 0; }
      else { pairs.push_back({0, lj, 1, lj}); si = 2; }

      if (!haveCorr[rp][cp])
      {
        Ciphertext ks = kk(pairs[0][0], pairs[0][1], pairs[0][2], pairs[0][3]);
        for (size_t p = 1; p < pairs.size(); ++p)
          ks = ks + kk(pairs[p][0], pairs[p][1], pairs[p][2], pairs[p][3]);
        bcorr[rp][cp] = b - ks;
        haveCorr[rp][cp] = true;
      }

      Ciphertext sum = k[si][lj] * x[r + si][c + lj];
      Ciphertext xs;
      for (size_t p = 0; p < pairs.size(); ++p)
      {
        int i1 = pairs[p][0], j1 = pairs[p][1], i2 = pairs[p][2], j2 = pairs[p][3];
        Ciphertext t = (k[i1][j1] + x[r + i2][c + j2]) * (k[i2][j2] + x[r + i1][c + j1]);
        sum = sum + t;
        Ciphertext q = xx(r + i1, c + j1, r + i2, c + j2);
        if (p == 0) xs = q;
        else xs = xs + q;
      }
      Ciphertext y = sum + bcorr[rp][cp] - xs;
      y.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}

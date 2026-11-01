vector<Ciphertext> dataT(const vector<Ciphertext> &d, int n)
{
  if (n == 4)
  {
    Ciphertext mm = d[3];
    Ciphertext ma = d[2] - d[3];
    Ciphertext mc = d[4] - d[3];
    Ciphertext am = d[1] - d[3];
    Ciphertext cm = d[5] - d[3];
    Ciphertext aa = (d[0] - d[1]) - ma;
    Ciphertext ac = (d[2] - d[1]) - mc;
    Ciphertext ca = (mc - ma) - cm;
    return {mm, ma, mc, am, aa, ac, cm, ca};
  }
  // n == 2: p0 d1, p1 d0-d1, p2 d2-d1, p3 d2, p4 d3
  return {d[1], d[0] - d[1], d[2] - d[1], d[2], d[3]};
}

vector<Ciphertext> kerT(const vector<Ciphertext> &g, int n)
{
  if (n == 4)
  {
    Ciphertext am = g[0] + g[1];
    Ciphertext mm = am + g[2];
    Ciphertext ma = g[0] + g[2];
    return {mm, ma, g[1], am, g[0], g[1], g[2], g[2]};
  }
  return {g[0] + g[1], g[0], g[1], g[2], g[2]};
}

vector<Ciphertext> outT(const vector<Ciphertext> &m, int n)
{
  if (n == 4)
  {
    // order: mm, ma, mc, am, aa, ac, cm, ca
    Ciphertext s1 = m[0] + m[1];
    Ciphertext s2 = m[0] + m[2];
    Ciphertext y0 = s1 + m[3] + m[4];
    Ciphertext y1 = s2 + m[3] + m[5];
    Ciphertext y2 = s1 + m[6] + m[7];
    Ciphertext y3 = s2 + m[6];
    return {y0, y1, y2, y3};
  }
  return {m[0] + m[1] + m[3], m[0] + m[2] + m[4]};
}

void fhe()
{
  const int W = WIDTH;
  const int n = W - 2;
  const int P = (n == 4) ? 8 : 5;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  // kernel transform (rows then columns)
  vector<vector<Ciphertext>> k1(3);
  for (int a = 0; a < 3; ++a) k1[a] = kerT(k[a], n);
  vector<vector<Ciphertext>> K(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q)
  {
    vector<Ciphertext> col(3);
    for (int a = 0; a < 3; ++a) col[a] = k1[a][q];
    vector<Ciphertext> t = kerT(col, n);
    for (int p = 0; p < P; ++p) K[p][q] = t[p];
  }

  // data transform
  vector<vector<Ciphertext>> d1(W);
  for (int i = 0; i < W; ++i) d1[i] = dataT(x[i], n);
  vector<vector<Ciphertext>> D(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q)
  {
    vector<Ciphertext> col(W);
    for (int i = 0; i < W; ++i) col[i] = d1[i][q];
    vector<Ciphertext> t = dataT(col, n);
    for (int p = 0; p < P; ++p) D[p][q] = t[p];
  }

  // products
  vector<vector<Ciphertext>> M(P, vector<Ciphertext>(P));
  for (int p = 0; p < P; ++p)
    for (int q = 0; q < P; ++q)
      M[p][q] = K[p][q] * D[p][q];
  // bias goes into the product shared by every output
  M[0][0] = M[0][0] + b;

  // output transform
  vector<vector<Ciphertext>> Z(P);
  for (int p = 0; p < P; ++p) Z[p] = outT(M[p], n);
  for (int c = 0; c < n; ++c)
  {
    vector<Ciphertext> col(P);
    for (int p = 0; p < P; ++p) col[p] = Z[p][c];
    vector<Ciphertext> ys = outT(col, n);
    for (int r = 0; r < n; ++r)
      ys[r].set_output("y_" + to_string(r) + "_" + to_string(c));
  }
}

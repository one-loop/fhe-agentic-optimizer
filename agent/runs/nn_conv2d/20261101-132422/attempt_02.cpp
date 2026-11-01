// 1D bilinear algorithm tables: kernel coeffs (3), data coeffs (n+2), output membership
struct Alg {
  int P;
  vector<vector<int>> kc; // P x 3
  vector<vector<int>> dc; // P x (n+2)
  vector<vector<int>> oc; // n x P
};

Ciphertext lincomb(const vector<Ciphertext> &v, const vector<int> &coef)
{
  int first = -1;
  for (size_t i = 0; i < coef.size(); ++i)
    if (coef[i] > 0) { first = (int)i; break; }
  Ciphertext acc;
  bool have = false;
  if (first >= 0)
  {
    acc = v[first];
    for (int t = 1; t < coef[first]; ++t) acc = acc + v[first];
    have = true;
  }
  for (size_t i = 0; i < coef.size(); ++i)
  {
    if ((int)i == first || coef[i] == 0) continue;
    int cnt = coef[i] > 0 ? coef[i] : -coef[i];
    for (int t = 0; t < cnt; ++t)
    {
      if (!have)
      {
        acc = -v[i];
        have = true;
      }
      else if (coef[i] > 0) acc = acc + v[i];
      else acc = acc - v[i];
    }
  }
  return acc;
}

void fhe()
{
  const int W = WIDTH;
  const int n = W - 2;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  Alg A;
  if (n == 4)
  {
    A.P = 8;
    // order: mm, ma, mc, am, aa, ac, cm, ca
    A.kc = {{1,1,1},{1,0,1},{0,1,0},{1,1,0},{1,0,0},{0,1,0},{0,0,1},{0,0,1}};
    A.dc = {
      {0,0,0,1,0,0},
      {0,0,1,-1,0,0},
      {0,0,0,-1,1,0},
      {0,1,0,-1,0,0},
      {1,-1,-1,1,0,0},
      {0,-1,1,1,-1,0},
      {0,0,0,-1,0,1},
      {0,0,-1,1,1,-1}};
    A.oc = {
      {1,1,0,1,1,0,0,0},
      {1,0,1,1,0,1,0,0},
      {1,1,0,0,0,0,1,1},
      {1,0,1,0,0,0,1,0}};
  }
  else
  {
    // n == 2
    A.P = 5;
    A.kc = {{1,1,0},{1,0,0},{0,1,0},{0,0,1},{0,0,1}};
    A.dc = {{0,1,0,0},{1,-1,0,0},{0,-1,1,0},{0,0,1,0},{0,0,0,1}};
    A.oc = {{1,1,0,1,0},{1,0,1,0,1}};
  }
  const int P = A.P;

  // kernel transform: separable
  vector<vector<Ciphertext>> k1(3, vector<Ciphertext>(P));
  for (int a = 0; a < 3; ++a)
    for (int q = 0; q < P; ++q)
      k1[a][q] = lincomb(k[a], A.kc[q]);
  vector<vector<Ciphertext>> K(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q)
  {
    vector<Ciphertext> col(3);
    for (int a = 0; a < 3; ++a) col[a] = k1[a][q];
    for (int p = 0; p < P; ++p)
      K[p][q] = lincomb(col, A.kc[p]);
  }

  // data transform: separable
  vector<vector<Ciphertext>> d1(W, vector<Ciphertext>(P));
  for (int i = 0; i < W; ++i)
    for (int q = 0; q < P; ++q)
      d1[i][q] = lincomb(x[i], A.dc[q]);
  vector<vector<Ciphertext>> D(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q)
  {
    vector<Ciphertext> col(W);
    for (int i = 0; i < W; ++i) col[i] = d1[i][q];
    for (int p = 0; p < P; ++p)
      D[p][q] = lincomb(col, A.dc[p]);
  }

  // products
  vector<vector<Ciphertext>> M(P, vector<Ciphertext>(P));
  for (int p = 0; p < P; ++p)
    for (int q = 0; q < P; ++q)
      M[p][q] = K[p][q] * D[p][q];

  // output transform: separable
  vector<vector<Ciphertext>> Z(P, vector<Ciphertext>(n));
  for (int p = 0; p < P; ++p)
    for (int c = 0; c < n; ++c)
      Z[p][c] = lincomb(M[p], A.oc[c]);

  for (int r = 0; r < n; ++r)
    for (int c = 0; c < n; ++c)
    {
      vector<Ciphertext> col(P);
      for (int p = 0; p < P; ++p) col[p] = Z[p][c];
      Ciphertext y = b + lincomb(col, A.oc[r]);
      y.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
}

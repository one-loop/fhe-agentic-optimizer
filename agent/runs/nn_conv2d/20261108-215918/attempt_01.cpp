struct Algo1D {
  int m, n; // outputs, inputs
  vector<vector<int>> G, D, O;
};

static Algo1D make_algo(int m)
{
  Algo1D a;
  a.m = m; a.n = m + 2;
  if (m == 2) {
    a.G = {{1,0,0},{1,1,0},{0,1,0},{0,0,1},{0,0,1}};
    a.D = {{1,-1,0,0},{0,1,0,0},{0,-1,1,0},{0,0,1,0},{0,0,0,1}};
    a.O = {{1,0},{1,1},{0,1},{1,0},{0,1}};
  } else if (m == 4) {
    a.G = {{1,0,0},{1,1,0},{0,1,0},{1,0,1},{1,1,1},{0,1,0},{0,0,1},{0,0,1}};
    a.D = {{1,-1,-1,1,0,0},{0,1,0,-1,0,0},{0,-1,1,1,-1,0},{0,0,1,-1,0,0},
           {0,0,0,1,0,0},{0,0,0,-1,1,0},{0,0,-1,0,1,0},{0,0,0,-1,0,1}};
    a.O = {{1,0,0,0},{1,1,0,0},{0,1,0,0},{1,0,1,0},{1,1,1,1},{0,1,0,1},{0,0,1,0},{0,0,0,1}};
  } else {
    // generic direct algorithm: products g_j * d_{i+j}
    for (int i = 0; i < m; ++i)
      for (int j = 0; j < 3; ++j) {
        vector<int> g(3, 0), d(m + 2, 0), o(m, 0);
        g[j] = 1; d[i + j] = 1; o[i] = 1;
        a.G.push_back(g); a.D.push_back(d); a.O.push_back(o);
      }
  }
  return a;
}

static Ciphertext lincomb(const vector<Ciphertext> &v, const vector<int> &coef)
{
  int start = -1;
  for (size_t i = 0; i < coef.size(); ++i)
    if (coef[i] > 0) { start = (int)i; break; }
  Ciphertext acc;
  if (start >= 0) {
    acc = (coef[start] == 1) ? v[start] : v[start] * coef[start];
  } else {
    for (size_t i = 0; i < coef.size(); ++i)
      if (coef[i] != 0) { start = (int)i; break; }
    acc = (coef[start] == -1) ? -v[start] : v[start] * coef[start];
  }
  for (size_t i = 0; i < coef.size(); ++i) {
    if ((int)i == start || coef[i] == 0) continue;
    if (coef[i] == 1) acc = acc + v[i];
    else if (coef[i] == -1) acc = acc - v[i];
    else acc = acc + v[i] * coef[i];
  }
  return acc;
}

void fhe()
{
  const int W = WIDTH;
  const int M = W - 2;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  Algo1D A = make_algo(M);
  int P = (int)A.G.size();

  // kernel transform: KT[i][q] = sum_j Gq[j] k[i][j]; KK[p][q] = sum_i Gp[i] KT[i][q]
  vector<vector<Ciphertext>> KT(3, vector<Ciphertext>(P));
  for (int i = 0; i < 3; ++i)
    for (int q = 0; q < P; ++q)
      KT[i][q] = lincomb(k[i], A.G[q]);
  vector<vector<Ciphertext>> KK(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q) {
    vector<Ciphertext> col(3);
    for (int i = 0; i < 3; ++i) col[i] = KT[i][q];
    for (int p = 0; p < P; ++p)
      KK[p][q] = lincomb(col, A.G[p]);
  }

  // data transform
  vector<vector<Ciphertext>> XT(W, vector<Ciphertext>(P));
  for (int a = 0; a < W; ++a)
    for (int q = 0; q < P; ++q)
      XT[a][q] = lincomb(x[a], A.D[q]);
  vector<vector<Ciphertext>> XX(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q) {
    vector<Ciphertext> col(W);
    for (int a = 0; a < W; ++a) col[a] = XT[a][q];
    for (int p = 0; p < P; ++p)
      XX[p][q] = lincomb(col, A.D[p]);
  }

  // products
  vector<vector<Ciphertext>> PR(P, vector<Ciphertext>(P));
  for (int p = 0; p < P; ++p)
    for (int q = 0; q < P; ++q)
      PR[p][q] = KK[p][q] * XX[p][q];

  // output transform: U[p][c] = sum_q Oq[c] PR[p][q]
  vector<vector<Ciphertext>> U(P, vector<Ciphertext>(M));
  for (int p = 0; p < P; ++p)
    for (int c = 0; c < M; ++c) {
      vector<int> coef(P);
      for (int q = 0; q < P; ++q) coef[q] = A.O[q][c];
      U[p][c] = lincomb(PR[p], coef);
    }

  for (int r = 0; r < M; ++r)
    for (int c = 0; c < M; ++c) {
      vector<Ciphertext> col(P + 1);
      vector<int> coef(P + 1);
      col[0] = b; coef[0] = 1;
      for (int p = 0; p < P; ++p) { col[p + 1] = U[p][c]; coef[p + 1] = A.O[p][r]; }
      Ciphertext y = lincomb(col, coef);
      y.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
}

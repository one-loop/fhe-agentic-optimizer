vector<Ciphertext> dataT(const vector<Ciphertext> &d, int n)
{
  if (n == 4)
  {
    Ciphertext a = d[4] - d[2];
    Ciphertext p02 = d[0] - d[2];
    Ciphertext p02_2 = p02 + p02;
    Ciphertext r0 = (p02_2 + p02_2) + a;
    Ciphertext u = d[1] + d[2];
    Ciphertext u2 = u + u;
    Ciphertext r1 = (d[3] + d[4]) - (u2 + u2);
    Ciphertext v = d[1] - d[2];
    Ciphertext v2 = v + v;
    Ciphertext r2 = (v2 + v2) + (d[4] - d[3]);
    Ciphertext e = d[3] - d[1];
    Ciphertext e2 = e + e;
    Ciphertext r3 = a + e2;
    Ciphertext r4 = a - e2;
    Ciphertext r5 = (d[5] - d[3]) - (e2 + e2);
    return {r0, r1, r2, r3, r4, r5};
  }
  return {d[1], d[0] - d[1], d[2] - d[1], d[2], d[3]};
}

vector<Ciphertext> kerT(const vector<Ciphertext> &g, int n)
{
  if (n == 4)
  {
    Ciphertext g0_2 = g[0] + g[0];
    Ciphertext g0_4 = g0_2 + g0_2;
    Ciphertext t0 = g0_4 + g0_2;            // 6 g0
    Ciphertext g2_2 = g[2] + g[2];
    Ciphertext g2_4 = g2_2 + g2_2;
    Ciphertext g2_8 = g2_4 + g2_4;
    Ciphertext g2_16 = g2_8 + g2_8;
    Ciphertext t5 = g2_16 + g2_8;           // 24 g2
    Ciphertext g12 = g[1] + g[1];
    Ciphertext g14 = g12 + g12;
    Ciphertext s4 = g0_4 + g2_4;
    Ciphertext t1 = s4 + g14;               // 4(g0+g1+g2)
    Ciphertext t2 = s4 - g14;               // 4(g0-g1+g2)
    Ciphertext h = g[0] + g2_4;
    Ciphertext t3 = h + g12;
    Ciphertext t4 = h - g12;
    return {t0, t1, t2, t3, t4, t5};
  }
  return {g[0] + g[1], g[0], g[1], g[2], g[2]};
}

vector<Ciphertext> outT(const vector<Ciphertext> &m, int n)
{
  if (n == 4)
  {
    Ciphertext dm = m[2] - m[1];
    Ciphertext sm = m[1] + m[2];
    Ciphertext d34 = m[3] - m[4];
    Ciphertext s34 = m[3] + m[4];
    Ciphertext y0 = (m[0] + s34) - sm;
    Ciphertext d2 = d34 + d34;
    Ciphertext y1 = dm + d2;
    Ciphertext s2 = s34 + s34;
    Ciphertext y2 = (s2 + s2) - sm;
    Ciphertext d4 = d2 + d2;
    Ciphertext y3 = (dm + (d4 + d4)) + m[5];
    return {y0, y1, y2, y3};
  }
  return {m[0] + m[1] + m[3], m[0] + m[2] + m[4]};
}

void fhe()
{
  const int W = WIDTH;
  const int n = W - 2;
  const int P = (n == 4) ? 6 : 5;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
    {
      Ciphertext kin("k_" + to_string(r) + "_" + to_string(c));
      if (n == 4)
        k[r][c] = kin * 260779; // inverse of 576 mod 786433
      else
        k[r][c] = kin;
    }

  Ciphertext b("b");

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

  vector<vector<Ciphertext>> M(P, vector<Ciphertext>(P));
  for (int p = 0; p < P; ++p)
    for (int q = 0; q < P; ++q)
      M[p][q] = K[p][q] * D[p][q];
  if (n == 4)
    M[1][1] = M[1][1] + b;
  else
    M[0][0] = M[0][0] + b;

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

static const long long TM = 786433;
static int md(long long a) { a %= TM; if (a < 0) a += TM; return (int)a; }

static vector<Ciphertext> data43(const vector<Ciphertext> &d)
{
  vector<Ciphertext> r(6);
  Ciphertext d1x4 = d[1] * 4;
  Ciphertext d2x4 = d[2] * 4;
  r[1] = (d[3] + d[4]) - (d1x4 + d2x4);
  r[2] = (d[4] - d[3]) + (d1x4 - d2x4);
  Ciphertext w = d[4] - d[2];
  r[0] = (d[0] - d[2]) * 4 + w;
  Ciphertext v = d[3] - d[1];
  Ciphertext u = v * 2;
  r[3] = w + u;
  r[4] = w - u;
  r[5] = (d[5] - d[3]) - v * 4;
  return r;
}

// first stage: scale folded into constants
static vector<Ciphertext> kern43_first(const vector<Ciphertext> &g, long long inv)
{
  vector<Ciphertext> s(6);
  s[0] = g[0] * md(6 * inv);
  Ciphertext e = g[0] + g[2];
  s[1] = (e + g[1]) * md(4 * inv);
  s[2] = (e - g[1]) * md(4 * inv);
  Ciphertext g1x2 = g[1] * md(2 * inv);
  Ciphertext f = g[0] * md(inv) + g[2] * md(4 * inv);
  s[3] = f + g1x2;
  s[4] = f - g1x2;
  s[5] = g[2] * md(24 * inv);
  return s;
}

static vector<Ciphertext> kern43(const vector<Ciphertext> &g)
{
  vector<Ciphertext> s(6);
  s[0] = g[0] * 6;
  Ciphertext e = g[0] + g[2];
  s[1] = (e + g[1]) * 4;
  s[2] = (e - g[1]) * 4;
  Ciphertext g1x2 = g[1] * 2;
  Ciphertext f = g[0] + g[2] * 4;
  s[3] = f + g1x2;
  s[4] = f - g1x2;
  s[5] = g[2] * 24;
  return s;
}

static vector<Ciphertext> out43(const vector<Ciphertext> &m)
{
  vector<Ciphertext> y(4);
  Ciphertext a = m[1] + m[2];
  Ciphertext ds = m[2] - m[1];
  Ciphertext p = m[3] + m[4];
  Ciphertext q = m[3] - m[4];
  y[0] = (m[0] + p) - a;
  y[1] = ds + q * 2;
  y[2] = p * 4 - a;
  y[3] = (ds + m[5]) + q * 8;
  return y;
}

static vector<Ciphertext> data23(const vector<Ciphertext> &d)
{
  vector<Ciphertext> r(4);
  r[0] = d[0] - d[2];
  r[1] = d[1] + d[2];
  r[2] = d[2] - d[1];
  r[3] = d[1] - d[3];
  return r;
}

static vector<Ciphertext> kern23_first(const vector<Ciphertext> &g, long long inv)
{
  vector<Ciphertext> s(4);
  s[0] = g[0] * md(2 * inv);
  Ciphertext e = g[0] + g[2];
  s[1] = (e + g[1]) * md(inv);
  s[2] = (e - g[1]) * md(inv);
  s[3] = g[2] * md(2 * inv);
  return s;
}

static vector<Ciphertext> kern23(const vector<Ciphertext> &g)
{
  vector<Ciphertext> s(4);
  s[0] = g[0] * 2;
  Ciphertext e = g[0] + g[2];
  s[1] = e + g[1];
  s[2] = e - g[1];
  s[3] = g[2] * 2;
  return s;
}

static vector<Ciphertext> out23(const vector<Ciphertext> &m)
{
  vector<Ciphertext> y(2);
  y[0] = m[0] + m[1] + m[2];
  y[1] = m[1] - m[2] - m[3];
  return y;
}

void fhe()
{
  const int W = WIDTH;
  const int M = W - 2;
  const int P = M + 2;
  const long long INV = (M == 4) ? 260779LL : 589825LL;

  vector<vector<Ciphertext>> x(W, vector<Ciphertext>(W));
  for (int r = 0; r < W; ++r)
    for (int c = 0; c < W; ++c)
      x[r][c] = Ciphertext("x_" + to_string(r) + "_" + to_string(c));

  vector<vector<Ciphertext>> k(3, vector<Ciphertext>(3));
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 3; ++c)
      k[r][c] = Ciphertext("k_" + to_string(r) + "_" + to_string(c));

  Ciphertext b("b");

  vector<vector<Ciphertext>> KT(3);
  for (int i = 0; i < 3; ++i)
    KT[i] = (M == 4) ? kern43_first(k[i], INV) : kern23_first(k[i], INV);
  vector<vector<Ciphertext>> KK(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q) {
    vector<Ciphertext> col(3);
    for (int i = 0; i < 3; ++i) col[i] = KT[i][q];
    vector<Ciphertext> t = (M == 4) ? kern43(col) : kern23(col);
    for (int p = 0; p < P; ++p) KK[p][q] = t[p];
  }

  vector<vector<Ciphertext>> XT(W);
  for (int a = 0; a < W; ++a) XT[a] = (M == 4) ? data43(x[a]) : data23(x[a]);
  vector<vector<Ciphertext>> XX(P, vector<Ciphertext>(P));
  for (int q = 0; q < P; ++q) {
    vector<Ciphertext> col(W);
    for (int a = 0; a < W; ++a) col[a] = XT[a][q];
    vector<Ciphertext> t = (M == 4) ? data43(col) : data23(col);
    for (int p = 0; p < P; ++p) XX[p][q] = t[p];
  }

  vector<vector<Ciphertext>> PR(P, vector<Ciphertext>(P));
  for (int p = 0; p < P; ++p)
    for (int q = 0; q < P; ++q)
      PR[p][q] = KK[p][q] * XX[p][q];

  PR[1][1] = PR[1][1] + b;

  vector<vector<Ciphertext>> U(P);
  for (int p = 0; p < P; ++p) U[p] = (M == 4) ? out43(PR[p]) : out23(PR[p]);

  for (int c = 0; c < M; ++c) {
    vector<Ciphertext> col(P);
    for (int p = 0; p < P; ++p) col[p] = U[p][c];
    vector<Ciphertext> Y = (M == 4) ? out43(col) : out23(col);
    for (int r = 0; r < M; ++r) {
      Ciphertext y = Y[r];
      y.set_output("y_" + to_string(r) + "_" + to_string(c));
    }
  }
}

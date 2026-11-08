I don't have a better kernel to propose, so this is attempt 3's kernel again, unchanged.

I can't find a cheaper division-free scheme. For four outputs with three taps, the cheapest scheme I can build without division is the two-level Karatsuba one at 8 multiplications. Nested over rows and columns, that gives 64 for the 6×6 case, down from 144. The lower-rank 6-multiplication schemes need division by 2 and 3 modulo the plaintext modulus. I can't use those, because the modulus isn't given and integer constants must be exact. The other splits I tried cost more: the polyphase split costs 10 and a 3-way split costs 9. Reordering the zero-padded tap gives the same count.

The remaining cost is the 64 relinearized multiplications. The 265 additions are only about 80 ms of the 3224 ms, so trimming them would not clear the 3% bar.

```cpp
typedef vector<Ciphertext> V;
typedef function<V(const V &, const V &)> Mul;

static V vadd(const V &a, const V &b)
{
  V r(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    r[i] = a[i] + b[i];
  return r;
}
static V vsub(const V &a, const V &b)
{
  V r(a.size());
  for (size_t i = 0; i < a.size(); ++i)
    r[i] = a[i] - b[i];
  return r;
}

// 4 outputs, 3 taps; d has 6 entries, g has 3 entries; 8 mults
static vector<V> toep(const vector<V> &d, const vector<V> &g, Mul M)
{
  V g01 = vadd(g[0], g[1]);
  V ga = vadd(g[0], g[2]);
  V gb = vadd(ga, g[1]);
  V t0, t1, w0, w1;
  {
    V e0 = vsub(d[0], d[2]), e1 = vsub(d[1], d[3]), e2 = vsub(d[2], d[4]);
    V q1 = M(vsub(e0, e1), g[0]);
    V q2 = M(e1, g01);
    V q3 = M(vsub(e2, e1), g[1]);
    t0 = vadd(q1, q2);
    t1 = vadd(q2, q3);
  }
  {
    const V &e0 = d[2], &e1 = d[3], &e2 = d[4];
    V q1 = M(vsub(e0, e1), ga);
    V q2 = M(e1, gb);
    V q3 = M(vsub(e2, e1), g[1]);
    w0 = vadd(q1, q2);
    w1 = vadd(q2, q3);
  }
  V u0 = M(vsub(d[4], d[2]), g[2]);
  V u1 = M(vsub(d[5], d[3]), g[2]);
  vector<V> out(4);
  out[0] = vadd(t0, w0);
  out[1] = vadd(t1, w1);
  out[2] = vadd(w0, u0);
  out[3] = vadd(w1, u1);
  return out;
}

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

  if (out_w == 4)
  {
    Mul M1 = [](const V &a, const V &bb) { return V{a[0] * bb[0]}; };
    Mul M2 = [&](const V &a, const V &bb) {
      vector<V> da(a.size()), gb(bb.size());
      for (size_t i = 0; i < a.size(); ++i)
        da[i] = V{a[i]};
      for (size_t i = 0; i < bb.size(); ++i)
        gb[i] = V{bb[i]};
      vector<V> o = toep(da, gb, M1);
      V flat(o.size());
      for (size_t i = 0; i < o.size(); ++i)
        flat[i] = o[i][0];
      return flat;
    };
    vector<V> rows(6), kr(3);
    for (size_t r = 0; r < 6; ++r)
      rows[r] = x[r];
    for (size_t i = 0; i < 3; ++i)
      kr[i] = k[i];
    vector<V> res = toep(rows, kr, M2);
    for (size_t r = 0; r < 4; ++r)
      for (size_t c = 0; c < 4; ++c)
      {
        Ciphertext y = b + res[r][c];
        y.set_output("y_" + to_string(r) + "_" + to_string(c));
      }
    return;
  }

  auto t3 = [&](const Ciphertext &v0, const Ciphertext &v1, const Ciphertext &v2,
                const Ciphertext &h0, const Ciphertext &h1, const Ciphertext &hs,
                Ciphertext &o0, Ciphertext &o1) {
    Ciphertext q1 = (v0 - v1) * h0;
    Ciphertext q2 = v1 * hs;
    Ciphertext q3 = (v2 - v1) * h1;
    o0 = q1 + q2;
    o1 = q2 + q3;
  };

  Ciphertext k00_01 = k[0][0] + k[0][1];
  Ciphertext k10_11 = k[1][0] + k[1][1];
  Ciphertext m0 = k[0][0] + k[1][0];
  Ciphertext m1 = k[0][1] + k[1][1];
  Ciphertext ms = k00_01 + k10_11;
  Ciphertext k02_12 = k[0][2] + k[1][2];
  Ciphertext k20_21 = k[2][0] + k[2][1];

  for (size_t r = 0; r < out_h; r += 2)
  {
    for (size_t c = 0; c < out_w; c += 2)
    {
      Ciphertext y[2][2];
      Ciphertext A[3], B[3], C[3];
      for (size_t j = 0; j < 3; ++j)
      {
        A[j] = x[r][c + j] - x[r + 1][c + j];
        B[j] = x[r + 1][c + j];
        C[j] = x[r + 2][c + j] - x[r + 1][c + j];
      }
      Ciphertext P1[2], P2[2], P3[2];
      t3(A[0], A[1], A[2], k[0][0], k[0][1], k00_01, P1[0], P1[1]);
      t3(B[0], B[1], B[2], m0, m1, ms, P2[0], P2[1]);
      t3(C[0], C[1], C[2], k[1][0], k[1][1], k10_11, P3[0], P3[1]);
      for (size_t v = 0; v < 2; ++v)
      {
        y[0][v] = b + (P1[v] + P2[v]);
        y[1][v] = b + (P2[v] + P3[v]);
      }
      for (size_t v = 0; v < 2; ++v)
      {
        Ciphertext o0, o1;
        t3(x[r][c + v + 2], x[r + 1][c + v + 2], x[r + 2][c + v + 2],
           k[0][2], k[1][2], k02_12, o0, o1);
        y[0][v] += o0;
        y[1][v] += o1;
      }
      for (size_t u = 0; u < 2; ++u)
      {
        const size_t rr = r + 2 + u;
        Ciphertext o0, o1;
        t3(x[rr][c], x[rr][c + 1], x[rr][c + 2], k[2][0], k[2][1], k20_21, o0, o1);
        y[u][0] += o0;
        y[u][1] += o1;
        y[u][0] += x[rr][c + 2] * k[2][2];
        y[u][1] += x[rr][c + 3] * k[2][2];
      }
      for (size_t u = 0; u < 2; ++u)
        for (size_t v = 0; v < 2; ++v)
          y[u][v].set_output("y_" + to_string(r + u) + "_" + to_string(c + v));
    }
  }
}
```
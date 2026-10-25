class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int degree, int stage) : stage_{stage}, pow2_{t}
  {
    while ((1 << pow2_.size()) < degree)
      pow2_.push_back(pow2_.back() * pow2_.back());
  }

  Ciphertext eval(int offset, int degree) const
  {
    const Ciphertext &t = pow2_[0];
    if (degree == 1)
      return t * coeff(offset + 1);
    if (degree == 3)
      return t * coeff(offset + 1) + (t * coeff(offset + 3)) * pow2_[1];

    size_t j = 0;
    while ((2 << j) < degree)
      ++j;
    const int m = 1 << j;
    return eval(offset, m - 1) + eval(offset + m, degree - m) * pow2_[j];
  }

private:
  Plaintext coeff(int k) const { return Plaintext("s" + to_string(stage_) + "c" + to_string(k)); }

  int stage_;
  vector<Ciphertext> pow2_;
};

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
    t = OddPolynomial(t, 7, stage).eval(0, 7);

  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");

  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext xt = x * t;
  Ciphertext A = u * c3 + (t * c1) ;
  Ciphertext B = u * c7 + (t * c5);
  (void)A;
  (void)B;
  // q(u) form without plaintext-constant adds: use t-scaled terms instead
  // p3(t) = c1 t + c3 t u + c5 t u^2 + c7 t u^3
  // y = x*p3 + half*x = (x t) * q(u) + half * x, q = c1 + c3 u + c5 u^2 + c7 u^3
  // c1 + c3 u cannot add a bare constant, so distribute x t into the pieces:
  Ciphertext xc1 = xt * c1;
  Ciphertext xc3u = (xt * c3) * u;
  Ciphertext xc5 = xt * c5;
  Ciphertext xc7u = (xt * c7) * u;
  Ciphertext y = xc1 + xc3u + (xc5 + xc7u) * u2 + x * half;
  y.set_output("y");
}

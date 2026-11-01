class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int stage) : stage_{stage}, t_{t}
  {
    u_ = t * t;
    u2_ = u_ * u_;
  }

  Ciphertext eval() const
  {
    Ciphertext lo = t_ * coeff(1) + (t_ * coeff(3)) * u_;
    Ciphertext hi = t_ * coeff(5) + (t_ * coeff(7)) * u_;
    return lo + hi * u2_;
  }

private:
  Plaintext coeff(int k) const { return Plaintext("s" + to_string(stage_) + "c" + to_string(k)); }

  int stage_;
  Ciphertext t_;
  Ciphertext u_, u2_;
};

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = OddPolynomial(t, stage).eval();
  Plaintext half("half");
  Ciphertext xh = x * half;
  Ciphertext y = x * t + xh;
  y.set_output("y");
}

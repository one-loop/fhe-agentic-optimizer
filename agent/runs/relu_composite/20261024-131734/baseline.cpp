// Baseline: ReLU(x) = x * (1 + sign(x / 4)) / 2 on [-4, 4], exactly like
// benchmarks/relu_ckks. sign is a composite of three odd degree-7 minimax
// polynomials p3(p2(p1(.))); the plaintext scalar s<stage>c<k> is the
// coefficient of t^k in stage <stage> (1/4^k folded into stage 1, 1/2 into
// stage 3), and half = 0.5. Each stage is evaluated baby-step/giant-step:
// degree 1: c1 t; degree 3: c1 t + (c3 t) t^2; degree 7: low(t) + t^4 high(t).
class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int degree, int stage) : stage_{stage}, pow2_{t}
  {
    // pow2_[j] = t^(2^j)
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
  for (int stage = 1; stage <= 3; ++stage)
    t = OddPolynomial(t, 7, stage).eval(0, 7);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

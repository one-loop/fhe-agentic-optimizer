Stages 1 and 2 are unchanged. In stage 3 I use the odd form p3(t) = t·q(u) with u = t², and fold the final multiply by x into it:

y = (x·t)·q(u) + x·half

This needs one fewer ciphertext multiplication than the baseline, and the depth stays at 10. Here q(u) = (c1 + c3·u) + (c5 + c7·u)·u², and the x·half term is a cheap scalar plaintext multiply done at level 0.

```cpp
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
  Ciphertext lo = u * c3 + c1;
  Ciphertext hi = u * c7 + c5;
  Ciphertext q = lo + hi * u2;
  Ciphertext xt = x * t;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}
```
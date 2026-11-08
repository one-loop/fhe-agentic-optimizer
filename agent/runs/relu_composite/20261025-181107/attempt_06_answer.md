Idea: use the baseline for stages 1 and 2. For stage 3 and the final multiply, rewrite `y = x·(t·q(u) + half)` as `(x·t)·q(u) + x·half`, where `q(u) = (c1 + c3·u) + (c5 + c7·u)·u²` and `u = t²`. The product `x·t` is formed in parallel with `q`, so the depth stays at 10. The rewrite needs 5 ciphertext multiplications instead of 6 for this part, and it replaces four plaintext multiplies with two plaintext multiplies and plaintext adds. The terms added together sit at different levels, so CHEHAB can align them.

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

  auto c = [&](int k) { return Plaintext("s3c" + to_string(k)); };
  Plaintext half("half");
  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext A = u * c(3) + c(1);
  Ciphertext B = u * c(7) + c(5);
  Ciphertext q = A + B * u2;
  Ciphertext xt = x * t;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}
```
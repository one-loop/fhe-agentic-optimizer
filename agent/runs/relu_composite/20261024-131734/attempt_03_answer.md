The previous attempt distributed the terms and used more multiplies, not fewer. Adding a plaintext scalar to a ciphertext is legal, so stage 3 can use the compact form directly:
- A = c3·u + c1
- B = c7·u + c5
- q = A + B·u²
- y = (x·t)·q + half·x

This costs 5 ct-ct multiplies (u, u², x·t, B·u², (x·t)·q) instead of 6, and the depth still ends at 10.

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
  Ciphertext xt = x * t;
  Ciphertext A = u * c3 + c1;
  Ciphertext B = u * c7 + c5;
  Ciphertext q = A + B * u2;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}
```
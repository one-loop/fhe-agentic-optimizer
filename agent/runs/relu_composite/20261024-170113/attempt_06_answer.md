I have no further structural saving to offer. This is attempt 4's kernel again, with `square(...)` written explicitly for the squared terms. The circuit should be essentially identical to attempt 4's, which measured about 639 ms against a baseline of about 642 ms in the same alternating batches, not a confirmed win.

The other ideas I checked do not pay off:
- **Pre-scaling `x`:** moving the coefficient multiplications onto `x` turns plaintext multiplications into ciphertext multiplications and adds ops.
- **Cutting a stage to 4 multiplications:** that needs depth 4 per stage and exceeds the 10-level budget.

```cpp
class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int degree, int stage) : stage_{stage}, pow2_{t}
  {
    while ((1 << pow2_.size()) < degree)
      pow2_.push_back(square(pow2_.back()));
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
  Plaintext a("s3c1"), b("s3c3"), c("s3c5"), d("s3c7");
  Plaintext half("half");
  Ciphertext w = x * t;
  Ciphertext u = square(t);
  Ciphertext u2 = square(u);
  Ciphertext q = ((u * b) + a) + ((u * d) + c) * u2;
  Ciphertext y = w * q + x * half;
  y.set_output("y");
}
```
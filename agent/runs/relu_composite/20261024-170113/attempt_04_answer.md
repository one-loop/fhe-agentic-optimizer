Stage 3 and the final `x*h` multiplication can be merged, which saves one ciphertext multiplication. Writing `p3 = t*Q(u)` with `u = t²`, we have `y = (x*t)*Q(u) + half*x`.

- **Product:** `w = x*t` is a single multiplication.
- **Polynomial:** `Q(u) = (a + b*u) + (c + d*u)*u²`. It needs `u`, `u²` and one more multiplication, with only two scalar plaintext multiplications and two plaintext adds.
- **Depth:** `t` is at level 6, so `u` is at level 7, `u²` at level 8 and `Q` at level 9. The final `w*Q` lands exactly on level 10.
- **Count:** the last stage plus the final step now costs 5 ciphertext multiplications, down from 6.

Stages 1 and 2 are unchanged. I have not run this, so scale alignment is the main risk.

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
  Plaintext a("s3c1"), b("s3c3"), c("s3c5"), d("s3c7");
  Plaintext half("half");
  Ciphertext w = x * t;
  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext q = ((u * b) + a) + ((u * d) + c) * u2;
  Ciphertext y = w * q + x * half;
  y.set_output("y");
}
```
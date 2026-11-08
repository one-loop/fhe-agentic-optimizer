The first two stages stay as in the baseline (3 levels each, 5 ct-ct multiplies each). I only restructure stage 3 together with the final `x*h` multiply.

With t the stage-2 output and u = t², stage 3 plus the final product is

y = (x·t)·[(c1 + c3·u) + u²·(c5 + c7·u)] + half·x

- **Multiplies:** the baseline needs 6 ct-ct multiplies for these two steps (t², t⁴, three products, and `x*h`). This form needs 5: `x*t`, `u`, `u²`, `u²·(c5+c7u)` and the final product.
- **Depth:** `x*t` is at depth 7 and the bracket is at depth 9, so the final product lands at depth 10. That is the same budget as the baseline.
- **Constants:** `c1` is added as a plaintext. `half·x` is a scalar plaintext multiply that CHEHAB aligns for free.

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
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7"), half("half");
  Ciphertext xt = x * t;
  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext lo = (u * c3) + c1;
  Ciphertext hi = (u * c7) + c5;
  Ciphertext A = lo + u2 * hi;
  Ciphertext y = xt * A + x * half;
  y.set_output("y");
}
```
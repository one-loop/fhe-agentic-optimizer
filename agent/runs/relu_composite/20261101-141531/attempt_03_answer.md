Attempt 2 removed one ciphertext multiply, but only a cheap one at level 9. It also added a `half*x` term and 7 extra mod switches, so the timing came out even.

This version keeps the fused stage 3, y = (x·t)·Q(u) + ½x, but builds both uses of x from one value z = half·x:

y = ((z + z)·t)·Q(u) + z

- Only one ciphertext reaches the deep levels, so it gets a single mod-switch chain instead of two, which should cut the extra mod switches.
- The `half` multiply now serves both terms.
- The multiply counts stay at 15 ct-ct and 11 ct-pt.
- No new same-level additions are introduced: z sits at level 1 against the product at level 10.

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

  // Stage 3 fused with the final multiplication by x:
  // y = x*(t*Q(u) + 1/2) = ((z+z)*t)*Q(u) + z, z = x/2, u = t^2.
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext z = x * half;                    // level 1
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (u * c7 + c5);         // level 9
  Ciphertext q = (u * c3 + c1) + hi;          // level 9
  Ciphertext xt = (z + z) * t;                // level 7
  Ciphertext y = xt * q + z;                  // level 10
  y.set_output("y");
}
```
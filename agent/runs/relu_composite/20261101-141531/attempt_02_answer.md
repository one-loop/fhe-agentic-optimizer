Attempt 1 failed because the two level-3 terms of a stage always have different scale histories. So stages 1 and 2 keep the baseline evaluation.

The change is in stage 3, merged with the final multiply by x. Write p3(t) = t·Q(u) with u = t², where Q(u) = (c1 + c3·u) + u²·(c5 + c7·u). Then:

y = x·(p3(t) + ½) = (x·t)·Q(u) + half·x

Levels for stage 3 (input t at level 6):

| Value | Level |
|---|---|
| x·t | 7 (computed in parallel) |
| u | 7 |
| u² and c7·u | 8 |
| u²·(c5 + c7·u) | 9 |
| (x·t)·Q | 10 |

The terms that are added always sit at different levels:
- c1 + c3·u (level 8) meets u²·(…) at level 9.
- half·x sits at level 1 against the product at level 10.

So no scale mismatch is expected.

Expected savings, stage 3 plus the final multiply:

| Operation | Baseline | This kernel |
|---|---|---|
| ct-ct multiplies | 6 | 5 |
| ct-pt multiplies | 4 | 3 (c7·u, c3·u, half·x) |
| Plaintext adds | 1 | 2 (c1 and c5) |

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

  // Stage 3 fused with the final multiplication by x.
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (u * c7 + c5);         // level 9
  Ciphertext q = (u * c3 + c1) + hi;          // level 9
  Ciphertext xt = x * t;                      // level 7
  Ciphertext y = xt * q + x * half;           // level 10
  y.set_output("y");
}
```
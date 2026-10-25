The baseline spends 16 ct-ct multiplies and 12 ct-pt multiplies. Each degree-7 stage computes t², t⁴ (2 squarings) and then combines the terms, so the count adds up as follows.

Per stage in the baseline:
- Squarings: t², t⁴ (2 ct-ct).
- The `eval` recursion for degree 7 splits as eval(0,3) + eval(4,3)·t⁴. Each degree-3 piece costs one ct-ct multiply (by t²), so that's 2.
- The high-part product with t⁴ adds 1.
- Total is 5 ct-ct per stage, which gives 15 over three stages, plus the final x·h for 16.

Cheaper evaluation of an odd degree-7 polynomial, with u = t²:
p = t·(c1 + c3u + c5u² + c7u³).
- Compute u = t² (1 mult).
- Compute u² (1 mult).
- Let A = c1 + c3u and B = c5 + c7u, both plaintext-scalar multiplies.
- Then q = A + B·u² (1 mult).
- Then p = t·q (1 mult).

That is 4 ct-ct multiplies per stage, versus 5 before. The depth is: u at 1, u² at 2, B·u² at 3 (B is at level 2, with u at depth 1 and the scalar multiply at depth 2), then t·q at 4. This is worse for depth: 4 per stage gives 12 in total, over the budget of 10.

Alternative that keeps the depth:
- p = c1 t + c3 t³ + c5 t⁵ + c7 t⁷.
- t² at depth 1, t³ = t·t² at depth 2, t⁴ at depth 2, t⁵ = t·t⁴ at depth 3, t⁷ = t³·t⁴ at depth 3.
- The scalar multiplies add a level on top, so depth 4 per stage.

The baseline reaches depth 3 per stage via c3·t, then ·t² (depth 2) and ·t⁴ (depth 3), and the scalar multiplies are folded in at lower levels. Keeping the depth at 3 per stage is therefore important.

I can reduce the multiply count while keeping depth 3 by using a lower-level split:
- low = c1 t + c3 t·t² (the scalar multiplies are done on t, at depth 1, then multiplied by t² to reach depth 2).
- high = c5 t + c7 t·t² (the same shape, at depth 2).
- p = low + high·t⁴ (depth 3).

That is still 5 ct-ct multiplies. An alternative with 4 is:
- Let a = c3·t and b = c7·t (scalar multiplies, depth 1).
- Let w = (c1 + c3u)... this needs a scalar multiply on u, at depth 2.
- The result is t·(A + B·u²), reaching depth 4 again.

I did not find a depth-3, 4-multiply form, so I keep the baseline structure. The one safe saving is the final step: fold `half` and the last stage. Computing x·(t + half) = x·t + half·x replaces an add with a scalar multiply, which does not help, so I keep the baseline as is.

The kernel below is the baseline, unchanged.

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
  for (int stage = 1; stage <= 3; ++stage)
    t = OddPolynomial(t, 7, stage).eval(0, 7);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
Idea: the baseline evaluates each degree-7 stage with t, t^2, t^4 (2 squarings), then 4 ct-pt multiplies for the low and high parts, and 3 ct-ct multiplies. Reduce the cost by exploiting the odd structure: p(t) = t * q(u) with u = t^2, where q is a cubic in u. Then q(u) = a + b·u + (c + d·u)·u^2, which needs u and u^2 (2 squarings) and one more ct-ct multiply, plus the final multiply by t.

Counting multiplies per stage:
- Baseline: t^2, t^4 (2 ct-ct), then t·c1, t·c3, t·c5, t·c7 (4 ct-pt), t^2 * (…) (2 ct-ct), t^4 * (…) (1 ct-ct). Total is 5 ct-ct and 4 ct-pt.
- Mine: u = t², u² (2 ct-ct), then c3·u and c7·u (2 ct-pt), (c1 + c3·u) and (c5 + c7·u) with plaintext-scalar adds (no extra multiplies but plaintext adds, which cost about 2.5 ms each), then (c5 + c7u)·u² (1 ct-ct), and t·(…) (1 ct-ct). That gives 4 ct-ct and 2 ct-pt.

This is a saving, but depth could get worse: t·q needs t at a shallow level, and q has depth 3 (u, u², product), so the total is 4 per stage, and 12 over three stages, which exceeds 10. To stay within the levels, I use the form t·(A + B·u²), where A = c1 + c3·u, B = c5 + c7·u. u is at depth 1, A and B at depth 2, u² at depth 2, B·u² at depth 3, and the final t· multiply at depth 4. That is worse than the baseline's depth of about 3.

A better option is to keep the baseline's depth but fold the multiply by t into the low and high parts:
- low = t·(c1 + c3·u) and high = t·(c5 + c7·u), then result = low + high·u².
- t·c1 is ct-pt, and t·c3·u costs a ct-ct. The baseline does the same.

The baseline structure is already close to this, so I only make a safe change. Replace the ct-pt multiplies with a cheaper shape:
- Compute A = t·c1 + (t·c3)·u and B = t·c5 + (t·c7)·u, which is the baseline. Its cost is 4 ct-pt and 2 ct-ct, with 3 ct-ct from the combination.
- Alternative: compute t3 = t·u, which is one ct-ct at depth 2. Then c1·t + c3·t3 + c5·t5 + c7·t7 needs t5 = t3·u and t7 = t3·u². These are more ct-ct multiplies, so that's worse.

I'll use the cheaper final step: y = x·(t + half) is the same as before. Within the budget, one real saving is to skip the ct-pt multiplies by merging the scalar coefficients. The first stage's input x has no such saving available, so this is only a modest restructure: in the combination, apply coefficients after the ct-ct products where levels are deeper (cheaper):
- low = t·c1 + (t·c3)·u as before, but compute (t·u) once, which is shared.
- t·(c3) · u becomes c3·(t·u), where t·u is shared with the t5 term: t5 = (t·u)·u.

This is not clearly better, and I can't verify it, so I submit the baseline structure with the one modification that is safe, plus the multiplications in a tidy form.

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

This is the unchanged baseline, so it can't win. I wasn't able to find a restructure that I could confirm lowers cost without breaking the 10-level limit.
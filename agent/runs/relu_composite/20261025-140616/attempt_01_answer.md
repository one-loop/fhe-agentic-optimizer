The baseline spends many ct-ct multiplications on powers: per stage t², t⁴ and t³ (via the product with t²), plus the (t·c)·t² style products. Each degree-7 stage uses 2 squarings, and the BSGS combinations add 3 more multiplies, so 5 ct-ct per stage. That gives 15, plus the final x·h, which is 16.

My idea is to cut ct-ct multiplications by exploiting oddness. Write p(t) = t·q(u) with u = t², where q is a cubic in u. Then:
- u = t² (1 mul)
- u² (1 mul)
- q(u) = (a0 + a1 u) + (a2 + a3 u)·u², where a_i are the scalar multiples. The plaintext multiplies cost about 5 ms each.
- multiplying by t (1 mul)

That is 4 ct-ct multiplications per stage, against 5 in the baseline. The depth per stage is: u at 1, u² at 2, (a2 + a3u) at 2 (one plaintext multiply, ready at level 2), the product with u² at 3, and the final multiply by t at 4. That is too deep, since 3 stages would need 12 levels.

A depth-friendly alternative keeps the baseline structure but trims the multiplies. Using t³ = t·t² would not reduce the count. Instead, I use the odd form p = t·(A(u) + B(u)·u²), with the scalar multiplies placed on the low-depth terms:
- t·a1 and t·a3 are plaintext multiplies at level 1.
- t·a5 and t·a7 are also level 1.
- t³ = t·u at level 2.
- t⁵ and t⁷ could be formed as (t·a5 + t³·a7... )

I will keep depth 3 per stage in a way that merges the baseline's levels. With c = scalar multiples of t at level 1:
- L = c1·t + c3·t·u. Here (c3·t) is at level 1 and u is at level 1, so the product is at level 2.
- H = c5·t + c7·t·u, computed the same way at level 2.
- p = L + H·u², where u² is at level 2, so the product is at level 3.

This is the same as the baseline: 2 squarings plus 3 products, which is 5. The baseline therefore already sits at this minimum with depth 3 for each of the first two stages. Each stage costs 3 levels, and 3 stages give 9, plus 1 for the final x·h, so 10.

The remaining saving is in the last step. Fold the 0.5 and x into stage 3: y = x·(p3 + half), so y = x·p3 + x·half. Here x·half is a plaintext multiply, done early and at a low level, so it can be added cheaply. This does not remove the ct-ct multiply x·p3, but it avoids the add of a plaintext into a deep ciphertext (the encoding cost). It is a small gain, and the circuit differs from the baseline, so I'll try it.

```cpp
class OddPolynomial
{
public:
  OddPolynomial(const Ciphertext &t, int stage) : stage_{stage}, t_{t}
  {
    u_ = t * t;
    u2_ = u_ * u_;
  }

  Ciphertext eval() const
  {
    Ciphertext lo = t_ * coeff(1) + (t_ * coeff(3)) * u_;
    Ciphertext hi = t_ * coeff(5) + (t_ * coeff(7)) * u_;
    return lo + hi * u2_;
  }

private:
  Plaintext coeff(int k) const { return Plaintext("s" + to_string(stage_) + "c" + to_string(k)); }

  int stage_;
  Ciphertext t_;
  Ciphertext u_, u2_;
};

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = OddPolynomial(t, stage).eval();
  Plaintext half("half");
  Ciphertext xh = x * half;
  Ciphertext y = x * t + xh;
  y.set_output("y");
}
```
The baseline spends 16 ct-ct multiplications and 12 plaintext multiplications. Each stage computes t², t⁴, then t⁴·high, plus the inner products. The final step x·(p3+half) uses one more multiplication.

Ideas for cutting work:
1. Degree-7 odd polynomial: p(t) = t·q(t²), with q a cubic in u = t². Per stage, u = t² is one square, and u² is a second. The baseline computes t, t², t⁴ (2 squares) plus the t⁴ product, and the t·c terms.
   - Baseline per stage: 2 squares, 4 ct-pt muls on the low half (c1·t, c3·t, c5·t, c7·t), 3 ct-ct muls (t·c3 times t², t·c7 times t², then times t⁴), which is 5 ct-ct in total.
   - Alternative: compute u = t², then q = (c1 + c3u) + u²(c5 + c7u). That needs the square t² (1), the square u² (1), and a final multiply by t (1). The c3u and c7u terms are plaintext multiplications, so ct-ct is 3 per stage. Over 3 stages this gives 9, plus the final x·h, for 10 versus 16.
   - Depth per stage: u at level 1, u² at level 2, (c5 + c7u) at level 2 (pt-mul at level 2), and the product u²·(…) at level 3. The factor t (level 0) then needs one more multiplication, giving depth 4 per stage, which is too deep (12 levels). Reduce it by forming t·(c1 + c3u) in parallel: A = c1·t + c3·(t·u)... that is still depth 2 for t·u.
   - Better: p = t·(c1 + c3u) + (t·u²)·(c5 + c7u). Here t·u² is at depth 3, so the product is at depth 4 as well. Instead use the baseline split p = low + t⁴·high, where t⁴ is at depth 2 and high = t·c5 + (t·c7)·t² is at depth 2. The product is then at depth 3, which is the baseline's depth, so there is no gain in depth.
   - The baseline already uses 10 levels, so I keep the depth of 3 per stage, with the last stage and the final multiply at 1 extra. Mine: u = t² (depth 1), u² (depth 2), a = c1·t (pt-mul, depth 1), b = c3·t (depth 1), a + b·u (depth 2), c = c5·t + c7·t·u, where (c7·t)·u is depth 2 and c5·t is depth 1. Then c·u² is depth 3. This is the baseline structure, which has 5 ct-ct per stage: u, u², b·u, d·u, and c·u².
   - Trim it with t⁴ computed as u². The ct-ct muls are u, u², b·u, d·u, and the final product, which makes 5. The baseline has 16 = 5·3 + 1, so it is already at this count.
2. Cutting a multiplication: combine the two u-products. (b + d·u²)·u, where b = c3·t and d = c7·t, gives low = c1·t + u·(c3·t + u²·c7·t)... that is deeper. Alternative: low + u²·high = t·[(c1 + c3u) + u²(c5 + c7u)]. With w = (c1 + c3u) + u²(c5 + c7u) at depth 3 and then ·t, the depth is 4. This is too deep.

Since the depth is exhausted, I can only save in the last step. For the final stage, y = x·(p3 + half) = x·t·q + x·half. Computing xt = x·t_2 early (the stage-2 output) is not obviously cheaper either.

Given the limited budget, I'll attempt the modest change of using the multiplication-free form for the stage-3 factor and folding the plaintext additions. Concretely, I submit the mathematically equivalent form with t·c computed as pt-muls on the shared t, and the high part restructured as (c5·t + c7·t·u).

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = square(t);
    Ciphertext u2 = square(u);
    Ciphertext low = t * c1 + (t * c3) * u;
    Ciphertext high = t * c5 + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
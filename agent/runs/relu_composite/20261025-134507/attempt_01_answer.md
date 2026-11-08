Idea: the baseline spends 16 ct-ct multiplications and 12 ct-pt multiplications. Each degree-7 stage needs t², t⁴ (2 squarings) plus the products with the high parts. A cheaper evaluation of an odd polynomial keeps the depth at 3 per stage, so the total stays at 9 plus the final multiplication by x, which is 10 levels.

For an odd degree-7 polynomial, write p(t) = t·q(u) with u = t², where q(u) = c1 + c3u + c5u² + c7u³.
- Compute u = t² (1 ct-ct multiplication, depth 1).
- Compute u² (depth 2).
- q = (c1 + c3u) + u²(c5 + c7u). The scalar multiplications c3u and c7u are at depth 2, then multiplying by u² gives depth 3, then multiplying by t gives depth 4. That is too deep.

Alternative that keeps depth 3: p = t(c1 + c3u) + t³·u(c5 + c7u)... t³ = t·u is at depth 2, and u² is also at depth 2. So p = A + B·C, with
- A = c1·t + c3·(t·u) (t·u is at depth 2, and the scalar multiplication brings it to depth 3),
- B = t·u²... (that is t⁵ at depth 3), so I use t⁴·(c5·t + c7·t³) instead.

That is the baseline form, and its cost is: t² (1), t⁴ (1), t³ (1), and one more product for the high part, so 4 ct-ct multiplications per stage.

I tried cutting the multiplication count with t⁴ as a square, but it is the same as t²·t². The per-stage count of squarings is already 2, plus the t·t² product for the high part, plus the t⁴ product. The baseline has 5 per stage (16 in total, with 15 from the stages plus 1 final). Mine will have:
- u = t·t (1)
- u2 = u·u (1)
- t3 = t·u (1)
- high = c5·t + c7·t3, which are scalar multiplications, so no ct-ct cost
- p = c1·t + c3·t3 + u2·high (1)

That is 4 per stage, so 12 plus 1 for the final multiplication, 13 in total versus 16. The high part is at depth 3 (t3 at depth 2, scalar multiplication brings it to 3), so u2·high would be at depth 4, which is too deep. To avoid that, I fold the coefficient into the other factor: u2·(c7·t3) needs c7·t3 at depth 3 either way. Instead use t³ computed with the scalar folded in early: (c7·t)·u gives depth 2 (the scalar multiplication on t is at depth 1, then the product with u at depth 1 gives depth 2). Then the high part is c5·t + c7·t3 at depth 2, with c5·t at depth 1, and the final product with u2 (depth 2) lands at depth 3. For the low part, c3·t3 needs c3·t folded in before the multiplication by u, so the low part is also at depth 2. The scalar multiplications are cheap.

Per stage:
- a1 = c1·t
- a3 = c3·t, b5 = c5·t, b7 = c7·t (scalar multiplications at depth 1)
- u = t², u2 = u²
- low = a1 + a3·u (depth 2)
- high = b5 + b7·u (depth 2)
- p = low + high·u2 (depth 3)

This uses 4 ct-ct multiplications per stage, with more ct-pt multiplications (4 per stage). The final step is y = x·(t + half), which uses the last level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
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
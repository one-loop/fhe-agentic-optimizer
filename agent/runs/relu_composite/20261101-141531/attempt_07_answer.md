I could not find a structural saving beyond the fused stage, and attempts 3–6 all landed about 1% faster.

The obstacles:
- **Depth:** each degree-7 stage needs 5 ciphertext multiplies to stay within 3 levels. The 4-multiply t·q(u) form costs an extra level that the 10-level budget doesn't have.
- **Scale mismatch:** it blocks the cheaper plaintext-add forms everywhere except the fused last stage.

This last scheduling variant keeps attempt 5's best layout:
- **Stages 1 and 2:** c₁·t is folded in at each stage's deepest level.
- **Stage 3:** it stays fused with the final multiply, using constants as plaintext adds.

What changes is the multiplier: x·t is computed directly from x instead of (z+z). That drops one ciphertext add and lets the half·x multiply sit on its own short path.

```cpp
Ciphertext eval_stage(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;                                  // L+1
  Ciphertext t4 = u * u;                                 // L+2
  Ciphertext high = (t * c(7)) * u + t * c(5);           // L+2
  Ciphertext rest = t4 * high + (t * c(3)) * u;          // L+3
  return rest + t * c(1);                                // L+3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
    t = eval_stage(t, stage);

  // Stage 3 fused with the final multiplication by x:
  // y = (x*t)*Q(u) + x/2, u = t^2, Q(u) = c1 + c3 u + u^2 (c5 + c7 u).
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (u * c7 + c5);         // level 9
  Ciphertext q = (hi + u * c3) + c1;          // level 9
  Ciphertext xt = x * t;                      // level 7
  Ciphertext y = xt * q + x * half;           // level 10
  y.set_output("y");
}
```
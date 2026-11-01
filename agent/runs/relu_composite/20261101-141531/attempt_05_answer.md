Attempts 3 and 4 came in about 1% faster. That margin cannot be confirmed against timing noise, so the next step needs a structural change.

This version keeps the fused stage 3 with z = half·x shared between both uses of x. It changes two things:

- **Stages 1 and 2:** each stage builds t³·c₃ and t³·c₇ as (c·t)·u, but folds the c₁ term in last. This puts the c₁ alignment at the stage's deepest, cheapest level, as in attempt 4.
- **Stage 3:** both plaintext constants (c₁ and c₅) are now added at the deepest point where they can go. Q is built as c₃·u + u²·(c₅ + c₇·u) at level 9, and c₁ is added there. Plaintext additions encode at the operand's scale, so they need no match-scale step, and at level 9 they are cheaper.

All additions still combine operands at different levels or via plaintext adds, so no scale mismatch is expected.

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
  // y = x*(t*Q(u) + 1/2) = ((z+z)*t)*Q(u) + z, z = x/2, u = t^2,
  // Q(u) = c1 + c3 u + u^2 (c5 + c7 u), with c1 added last (plaintext add).
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext z = x * half;                    // level 1
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (u * c7 + c5);         // level 9
  Ciphertext q = (hi + u * c3) + c1;          // level 9
  Ciphertext xt = (z + z) * t;                // level 7
  Ciphertext y = xt * q + z;                  // level 10
  y.set_output("y");
}
```
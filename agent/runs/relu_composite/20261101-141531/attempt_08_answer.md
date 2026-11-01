I have not found a way to get the 3% needed to beat the baseline. Every kernel so far, including this one, sits at 15 ciphertext-ciphertext multiplies. Each degree-7 stage needs 5 multiplies to fit in 3 levels. The cheaper t·q(u) form needs a 4th level, which only fits in stage 3 by absorbing the final multiply by x. Stages 1 and 2 cannot use plaintext adds for their constants because CHEHAB reports a scale mismatch.

Attempts 3–7 all measured about 1% faster than the baseline in alternating timing, and none was confirmed. Attempt 5 timed best, at 316 ms against 320 ms.

This last attempt starts from attempt 5:
- **Stage 3:** the fused form stays, with z = half·x shared as (z+z)·t.
- **q:** built as ((c3·u + hi) + c1).
- **Stages 1 and 2:** the plaintext multiplies are reordered.

This gives a new circuit with the same multiply counts. It will probably measure about 1% faster again and not be confirmed.

```cpp
Ciphertext eval_stage(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;                                  // L+1
  Ciphertext t4 = u * u;                                 // L+2
  Ciphertext a3 = (t * c(3)) * u;                        // L+2
  Ciphertext high = t * c(5) + (t * c(7)) * u;           // L+2
  return (a3 + t4 * high) + t * c(1);                    // L+3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
    t = eval_stage(t, stage);

  // Stage 3 fused with the final multiplication by x:
  // y = ((z+z)*t)*Q(u) + z, z = x/2, u = t^2, Q(u) = c1 + c3 u + u^2 (c5 + c7 u).
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext z = x * half;                    // level 1
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (c5 + u * c7);         // level 9
  Ciphertext q = (u * c3 + hi) + c1;          // level 9
  Ciphertext xt = t * (z + z);                // level 7
  Ciphertext y = z + xt * q;                  // level 10
  y.set_output("y");
}
```
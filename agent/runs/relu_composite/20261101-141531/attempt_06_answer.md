Attempts 3–5 each landed about 1% faster, short of the 3% needed. I could not find a structural saving beyond the fused stage:

- **Multiplies:** each degree-7 stage needs 5 ciphertext-ciphertext multiplies to stay within 3 levels.
- **4-multiply form:** t·q(u) costs a fourth level.
- **Scale mismatch:** it blocks merging the low and high parts at the same level.

This is one more scheduling variant to test:

- **Stage 1** uses the baseline grouping: c1·t is aligned at level 2 into the low part, at the cheaper top levels.
- **Stage 2** keeps the attempt-5 grouping, folding c1·t in last at its deepest level.
- **Stage 3** keeps the fused form with z = half·x shared. q is built as ((c3·u + c1) + hi), so the c1 plaintext add happens at level 8.

```cpp
Ciphertext eval_stage_a(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;                                  // L+1
  Ciphertext t4 = u * u;                                 // L+2
  Ciphertext low = t * c(1) + (t * c(3)) * u;            // L+2
  Ciphertext high = t * c(5) + (t * c(7)) * u;           // L+2
  return low + t4 * high;                                // L+3
}

Ciphertext eval_stage_b(const Ciphertext &t, int stage)
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
  Ciphertext t = eval_stage_a(x, 1);
  t = eval_stage_b(t, 2);

  // Stage 3 fused with the final multiplication by x:
  // y = ((z+z)*t)*Q(u) + z, z = x/2, u = t^2, Q(u) = c1 + c3 u + u^2 (c5 + c7 u).
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
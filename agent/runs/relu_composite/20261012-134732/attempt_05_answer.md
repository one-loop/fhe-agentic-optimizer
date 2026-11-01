I'm going to stop here and submit attempt 2 as the final kernel. Attempt 4 measured 317.75 ms and was the same operation graph again, so further resubmissions of this structure are not worth the remaining attempts.

**Where things stand:** every variant so far reports 16 ciphertext-ciphertext multiplies, 12 plaintext multiplies, 9 `match_scale` and 9 `mod_switch`. Timings sit between 317.45 and 317.92 ms, which is run-to-run noise against the 318.38 ms baseline. The kernel is correct to about 2e-7.

**Why the count is stuck:** each degree-7 stage needs multiplicative depth 3, and the three stages plus the final `x * h` use all 10 levels. Within depth 3 I could not find an evaluation with fewer than 5 ciphertext multiplies per stage. A depth-4 stage pushes the total to 11, which is over budget.

**The one lead I did not test:** the 9 `match_scale` operations, roughly 45 ms in total, come from how CHEHAB aligns scales at additions. Removing them would require probing the compiler's alignment choices with variants of the additions, which I have not done.

```cpp
// Per stage (t = input, depth 3, 5 ciphertext multiplications):
//   u = t^2, u4 = t^4
//   low  = c1*t + (c3*t)*u          (= c1 t + c3 t^3)
//   high = c5*t + (c7*t)*u          (= c5 t + c7 t^3)
//   p(t) = low + u4*high            (= c1 t + c3 t^3 + c5 t^5 + c7 t^7)
Ciphertext relu_stage(const Ciphertext &t, int stage)
{
  auto coeff = [stage](int k) {
    return Plaintext("s" + to_string(stage) + "c" + to_string(k));
  };

  Ciphertext u = t * t;             // t^2, depth 1
  Ciphertext u4 = u * u;            // t^4, depth 2

  Ciphertext low = t * coeff(1) + (t * coeff(3)) * u;     // depth 2
  Ciphertext high = t * coeff(5) + (t * coeff(7)) * u;    // depth 2

  return low + high * u4;           // depth 3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = relu_stage(x, 1);
  t = relu_stage(t, 2);
  t = relu_stage(t, 3);

  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
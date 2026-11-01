I'm submitting attempt 2 as the final kernel. Attempt 6 measured 318.49 ms, the same operation graph as every other attempt, and timings across all of them sit within run-to-run noise of the baseline (318.38 ms).

**Why I'm not spending the last two attempts:**
- Every variant produces 16 ciphertext-ciphertext multiplies, 12 plaintext multiplies, 9 `match_scale` and 9 `mod_switch`.
- Depth fixes the structure: each degree-7 stage needs depth 3, and with the final `x * h` the total is exactly 10 levels. Within depth 3 I found no evaluation with fewer than 5 ciphertext multiplies per stage.
- The one remaining lead is the 9 `match_scale` operations (about 45 ms). Removing them requires understanding how CHEHAB aligns scales at additions, which I could only probe by compiling variants. I did not manage that within the attempts.

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
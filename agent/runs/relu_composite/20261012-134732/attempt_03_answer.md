The best result so far is attempt 2 at 317.45 ms, a 0.3% gain over the baseline. I don't have a promising change left that would move it meaningfully, so I'm stopping here rather than spending more attempts.

**Why further gains look out of reach:**
- Each stage needs depth 3, and the three stages plus the final `x * h` use all 10 levels. Any evaluation with depth 4 in a stage breaks the budget.
- I checked several re-associations of the degree-7 polynomial, including sharing `t*u` across the low and high halves, using `t^5` and `t^7` directly, and folding scalars into `u` first. Each one either still needs five ciphertext-ciphertext multiplications or pushes a stage to depth 4.
- The 9 `match_scale` operations are the other possible target, at about 5 ms each. Fixing them requires controlling how CHEHAB aligns scales at each addition, and I can't predict its decisions from the DSL alone.

**Final kernel (attempt 2):** the same structure as attempts 1 and 2, with `u = t^2` and `u4 = t^4` shared, the low and high halves each built from a scalar multiply on `t` followed by a product with `u`, and the stage output formed as `low + high*u4`.

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
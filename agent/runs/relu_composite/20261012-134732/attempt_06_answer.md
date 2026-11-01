I'm submitting attempt 2 as the final kernel. Attempt 5 measured 317.96 ms, the same operation graph as the rest, and I have no variant with a real chance of beating 317.45 ms.

**Summary of the search:**
- Every attempt produced the same operation counts: 16 ciphertext-ciphertext multiplies, 12 plaintext multiplies, 9 `match_scale`, 9 `mod_switch`.
- Timings ranged from 317.45 to 317.96 ms, against a 318.38 ms baseline. That spread is run-to-run noise, so the gain is about 0.3%.
- Depth fixes the structure: each degree-7 stage needs depth 3, and the three stages plus the final `x * h` use the full 10 levels. Within depth 3 I found no evaluation with fewer than 5 ciphertext multiplies per stage.

**Untested lead:** the 9 `match_scale` operations (roughly 45 ms) come from CHEHAB's scale alignment at additions. Removing them needs probing how the compiler aligns operands, which I did not do with the remaining attempts.

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
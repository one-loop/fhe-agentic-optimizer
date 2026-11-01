I'm going to stop iterating here. Attempt 3 measured 317.92 ms, which is within noise of attempt 2's 317.45 ms, and it was essentially the same kernel again. Repeating that structure won't move the number, and I don't have a tested alternative that reduces the operation count.

**What the profile says:** the kernel uses 16 ciphertext-ciphertext multiplies, 12 plaintext multiplies, 9 `match_scale` and 9 `mod_switch` operations. The 16 ciphertext multiplies are the dominant cost, and the depth budget fixes them at 5 per stage: each degree-7 stage needs depth 3, and the three stages plus the final `x * h` use all 10 levels. A depth-4 stage would exceed the budget, and I could not find a depth-3 evaluation with fewer than 5 ciphertext multiplies.

**What might still help, untested:** the 9 `match_scale` operations at roughly 5 ms each (about 45 ms total) are the only identified overhead I haven't been able to remove. Eliminating them requires knowing how CHEHAB decides scale alignment at each addition, which I can't determine from the DSL. A probe would need to compile variants of the additions and read the operation counts, and I'd rather not spend the remaining attempts guessing.

**Recommended final kernel:** attempt 2, which measured best at 317.45 ms, correct to about 2e-7 against the expected output.

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
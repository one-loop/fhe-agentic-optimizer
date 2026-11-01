Each stage currently computes the low part as `c1*t + (c3*t)*t^2`. That costs two plaintext multiplies, and the `c1*t` term (level 1) needs a scale alignment before it can be added at level 2. I rewrite the low part as `t * (c3*t^2 + c1)`:

- **Coefficient multiplies:** `c1` becomes a cheap plaintext add, so this saves one multiply and one alignment per stage.
- **Cost:** the number of ciphertext-ciphertext multiplies is unchanged at 5 per stage.
- **Depth:** stays at 3 per stage, and 10 overall.
- **Risk:** the low part now reaches level 3 with a different multiplication history from `t^4 * high`. CHEHAB may report a scale mismatch there, and if it does I will restructure.

```cpp
Ciphertext eval_stage(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;    // t^2, level 1
  Ciphertext t4 = u * u;   // t^4, level 2
  Ciphertext low = t * (u * c(3) + c(1));            // level 3
  Ciphertext high = t * c(5) + (t * c(7)) * u;       // level 2
  return low + high * t4;                            // level 3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = eval_stage(t, stage);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
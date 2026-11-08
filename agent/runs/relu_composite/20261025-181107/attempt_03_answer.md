The previous failure came from adding two values at level 3 that had different multiplication histories. This attempt goes back to the baseline structure, which meets at different levels, and uses `square()` for the power computations. That gives a different circuit with the same depth, and squaring is cheaper than a general ciphertext multiplication.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = square(t);
    Ciphertext u2 = square(u);
    Ciphertext low = t * c(1) + (t * c(3)) * u;
    Ciphertext high = t * c(5) + (t * c(7)) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
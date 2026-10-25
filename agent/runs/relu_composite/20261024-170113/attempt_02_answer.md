The idea here is to rewrite the low half of each stage as `t * (a + b*u)` with `u = t²`. It has the same depth, but it replaces one scalar plaintext multiplication per stage with a cheaper plaintext add. The high half stays unchanged, because the same rewrite would push a stage past depth 3. I haven't timed this, so the saving is an estimate.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
    Ciphertext low = t * ((u * c(3)) + c(1));
    Ciphertext high = (t * c(5)) + (t * c(7)) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
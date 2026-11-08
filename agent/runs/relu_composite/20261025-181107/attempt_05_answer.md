I couldn't find a restructure with fewer operations that also keeps scales aligned within 10 levels. This attempt is the baseline structure with the first power computed by `square()` and the second by a plain multiply, which gives a distinct circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = square(t);
    Ciphertext u2 = u * u;
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
I haven't found a way to cut multiplications within the depth limit, so this attempt only changes how the squares are formed. It keeps `u = t*t` but uses `square(u)` for t⁴. This yields a different mix of square and multiply operations, which may be slightly cheaper.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = t * t;
    Ciphertext u2 = square(u);
    Ciphertext low = t * c1 + (t * c3) * u;
    Ciphertext high = (t * c5) + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
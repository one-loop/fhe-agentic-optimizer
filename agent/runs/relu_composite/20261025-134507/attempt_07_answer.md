The distributed version failed on a scale mismatch: the two high-part terms had different multiplication counts (D⁶ vs D⁸) at the same level. I'm returning to the valid baseline structure and varying only how the squares are formed, using `square(t)` for t² and the plain product `u*u` for t⁴. This is another small circuit variation.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = square(t);
    Ciphertext u2 = u * u;
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
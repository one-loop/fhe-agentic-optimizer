I still haven't found a formulation with fewer multiplications at depth 3, so this is another structural variant. It uses `square()` for both powers and distributes u² over the high part. The high part becomes `(t*c5)*u2 + ((t*c7)*u)*u2`, which keeps the depth at 3 but changes the operation mix and the order of rescales and alignments.

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
    Ciphertext u2 = square(u);
    Ciphertext low = t * c1 + (t * c3) * u;
    Ciphertext hi5 = (t * c5) * u2;
    Ciphertext hi7 = ((t * c7) * u) * u2;
    t = low + (hi5 + hi7);
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```
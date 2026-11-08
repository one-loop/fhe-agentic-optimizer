The scale mismatch came from `lo = t * ((u*c3) + c1)`: its scale history differs from that of `hi*u2` at the same level. So I'm trying a different low part in stages 1 and 2:

lo = c1·t + c3·(t·u)

This takes the cubic term from the shared `t*u` product, so the plaintext multiply comes last. Its scale pattern should then match that of the other terms. Stage 3 stays as in attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
  {
    string s = "s" + to_string(stage) + "c";
    Plaintext c1(s + "1"), c3(s + "3"), c5(s + "5"), c7(s + "7");
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
    Ciphertext lo = (t * c1) + (t * c3) * u;
    Ciphertext hi = (t * c5) + (t * c7) * u;
    t = lo + hi * u2;
  }
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7"), half("half");
  Ciphertext xt = x * t;
  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext lo = (u * c3) + c1;
  Ciphertext hi = (u * c7) + c5;
  Ciphertext A = lo + u2 * hi;
  Ciphertext y = xt * A + x * half;
  y.set_output("y");
}
```
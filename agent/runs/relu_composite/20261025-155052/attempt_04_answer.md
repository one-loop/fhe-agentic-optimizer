Stage 3 keeps the regrouped form from attempt 3. In stages 1 and 2 I rewrite the low part as `t*(c1 + c3*u)`, which needs one ciphertext-plaintext multiply fewer per stage. The plaintext add replaces the `c1*t` multiply. It can hit a scale mismatch, but I'll try it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = square(t);
    Ciphertext u2 = square(u);
    Ciphertext low = t * (u * c3 + c1);
    Ciphertext high = t * c5 + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7"), half("half");
  Ciphertext u = square(t);
  Ciphertext u2 = square(u);
  Ciphertext xt = x * t;
  Ciphertext a = u * c3 + c1;
  Ciphertext b = u * c7 + c5;
  Ciphertext q = a + b * u2;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}
```
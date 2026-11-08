Stages 1 and 2 stay in the baseline form. In stage 3, the output is `x*(t*q(u)+half)`, where `q(u) = c1 + c3 u + c5 u² + c7 u³` and `t` is the stage-2 output. I regroup it as `(x*t)*q + x*half`. The product `x*t` is computed in parallel with `u` and `u²`, so `q` is evaluated by Horner in `u` and stage 3 plus the final multiply fit in 4 levels. This drops one ciphertext-ciphertext multiplication and some plaintext multiplications.

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
    Ciphertext low = t * c1 + (t * c3) * u;
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
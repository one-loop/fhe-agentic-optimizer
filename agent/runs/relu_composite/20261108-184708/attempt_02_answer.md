The 15-multiply version of attempt 1 only matched the baseline within noise. This attempt cuts plaintext work in stages 1 and 2 as well.

In each stage the low part c1·t + c3·t³ can be written as t·(c1 + c3·t²). The scalar multiply c3·t² and a plaintext add of c1 replace the baseline's two scalar multiplies. This low part lands at depth 3, the same as the high-times-t⁴ term, so the total depth is unchanged. The high part and stage 3 stay as before.

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
    Ciphertext lo = t * ((u * c3) + c1);
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
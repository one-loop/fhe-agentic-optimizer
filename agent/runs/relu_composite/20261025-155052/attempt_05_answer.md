I could not find a further safe improvement. The `t*(c1 + c3*u)` rewrite in stages 1 and 2 hits the scale mismatch, because the low part lands at level 3 with a different scale from `high*u2`. Other regroupings I considered either exceed 10 levels or add multiplications. I am therefore resubmitting the attempt-3 kernel. It saves one ciphertext-ciphertext multiplication and one plaintext multiplication, and it measured about 1.5% faster than the baseline, below the 3% needed to count as confirmed.

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
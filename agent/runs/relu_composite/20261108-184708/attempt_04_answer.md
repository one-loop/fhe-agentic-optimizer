Attempt 3 was identical to attempt 1 once CHEHAB normalized it. Here I change the shape of the low part in stages 1 and 2: lo = c1·t + c3·(t·u). The scalar multiply by c3 then runs after the ct-ct product, at a lower level, where it is cheaper. Stage 3 stays as in attempt 1.

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
    Ciphertext t3 = t * u;
    Ciphertext lo = (t * c1) + t3 * c3;
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
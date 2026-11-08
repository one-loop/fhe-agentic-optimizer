The last stage and the final multiplication by x can be merged. Let p3(t) = t·Q(u) with Q = (c1 + c3u) + u²(c5 + c7u). Then y = (x·t)·Q + x·half. The product x·t can be computed as soon as stage 2's output t is ready. Q needs depth 3, so the final product lands at level 10, which is allowed. For stages 1 and 2 I keep the baseline evaluation.

This saves one ct-ct multiplication and one ct-pt multiplication compared with the baseline's last stage plus final multiply. The two terms of Q meet at different levels (2 and 3), so CHEHAB can align them for free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
    Ciphertext low = t * c1 + (t * c3) * u;
    Ciphertext high = (t * c5) + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext qlow = (u * c3) + c1;
  Ciphertext qhigh = (u * c7) + c5;
  Ciphertext Q = qlow + qhigh * u2;
  Ciphertext xt = x * t;
  Ciphertext y = xt * Q + x * half;
  y.set_output("y");
}
```
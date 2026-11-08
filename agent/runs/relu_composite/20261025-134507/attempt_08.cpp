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

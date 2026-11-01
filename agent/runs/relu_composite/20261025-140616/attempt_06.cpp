Ciphertext stage_eval(const Ciphertext &t, int stage)
{
  auto coeff = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = square(t);
  Ciphertext u2 = square(u);
  Ciphertext lo = t * (u * coeff(3) + coeff(1));
  Ciphertext hi = t * coeff(5) + (t * coeff(7)) * u;
  return lo + hi * u2;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
    t = stage_eval(t, stage);

  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");

  Ciphertext u = square(t);
  Ciphertext u2 = square(u);
  Ciphertext lo = u * c3 + c1;
  Ciphertext hi = u * c7 + c5;
  Ciphertext q = lo + hi * u2;
  Ciphertext xt = x * t;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}

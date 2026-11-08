void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = square(t);
    Ciphertext u2 = square(u);
    Ciphertext low = t * c(1) + (t * c(3)) * u;
    Ciphertext high = t * c(5) + (t * c(7)) * u;
    t = low + high * u2;
  }

  auto c = [&](int k) { return Plaintext("s3c" + to_string(k)); };
  Plaintext half("half");
  Ciphertext u = square(t);
  Ciphertext u2 = square(u);
  Ciphertext A = u * c(3) + c(1);
  Ciphertext B = u * c(7) + c(5);
  Ciphertext q = A + B * u2;
  Ciphertext xt = x * t;
  Ciphertext y = xt * q + x * half;
  y.set_output("y");
}

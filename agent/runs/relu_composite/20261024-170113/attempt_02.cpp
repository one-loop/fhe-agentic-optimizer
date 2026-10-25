void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
    Ciphertext low = t * ((u * c(3)) + c(1));
    Ciphertext high = (t * c(5)) + (t * c(7)) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

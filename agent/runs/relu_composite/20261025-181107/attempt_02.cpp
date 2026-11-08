void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
    Ciphertext u = square(t);
    Ciphertext u2 = square(u);
    Ciphertext t3 = t * u;
    Ciphertext low = t * c(1) + t3 * c(3);
    Ciphertext high = t * c(5) + (t * c(7)) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

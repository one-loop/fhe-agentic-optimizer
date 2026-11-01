Ciphertext eval_stage(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;    // t^2, level 1
  Ciphertext t4 = u * u;   // t^4, level 2
  Ciphertext low = t * (u * c(3) + c(1));            // level 3
  Ciphertext high = t * c(5) + (t * c(7)) * u;       // level 2
  return low + high * t4;                            // level 3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = eval_stage(t, stage);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

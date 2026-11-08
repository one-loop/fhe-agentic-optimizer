Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_eval(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;                       // L1
  Ciphertext c7u = u * coeff(stage, 7);       // L1
  Ciphertext c7u2 = c7u * u;                  // L2
  Ciphertext c5u = u * coeff(stage, 5);       // L1
  Ciphertext inner = (c7u2 + c5u) + coeff(stage, 3); // L2
  Ciphertext t3 = t * u;                      // L2
  Ciphertext hi = t3 * inner;                 // L3
  Ciphertext lo = t * coeff(stage, 1);        // L1
  return hi + lo;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = stage_eval(t, stage);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_fast(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;
  Ciphertext c7u = u * coeff(stage, 7);
  Ciphertext c7u2 = c7u * u;
  Ciphertext c5u = u * coeff(stage, 5);
  Ciphertext inner = (c7u2 + c5u) + coeff(stage, 3);
  Ciphertext t3 = t * u;
  Ciphertext hi = t3 * inner;
  Ciphertext lo = t * coeff(stage, 1);
  return hi + lo;
}

Ciphertext stage_base(const Ciphertext &t, int stage)
{
  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext low = t * coeff(stage, 1) + (t * coeff(stage, 3)) * t2;
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * t2;
  return low + high * t4;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  t = stage_fast(t, 1);
  t = stage_fast(t, 2);
  t = stage_base(t, 3);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}

Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
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
  Ciphertext X1 = x * coeff(3, 1);
  Ciphertext X3 = x * coeff(3, 3);
  Ciphertext X5 = x * coeff(3, 5);
  Ciphertext X7 = x * coeff(3, 7);
  Plaintext half("half");
  Ciphertext xh = x * half;

  Ciphertext t = stage_base(x, 1);
  t = stage_base(t, 2);

  Ciphertext u = t * t;
  Ciphertext u2 = u * u;
  Ciphertext A = X3 * u + X1;
  Ciphertext low = t * A;
  Ciphertext H = X5 * t + (X7 * t) * u;
  Ciphertext high = H * u2;
  Ciphertext y = (low + high) + xh;
  y.set_output("y");
}

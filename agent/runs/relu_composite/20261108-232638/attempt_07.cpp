Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_base(const Ciphertext &t, int stage)
{
  Ciphertext t2 = square(t);
  Ciphertext t4 = square(t2);
  Ciphertext high = (t * coeff(stage, 7)) * t2 + t * coeff(stage, 5);
  Ciphertext top = high * t4;                       // L3
  Ciphertext mid = (t * coeff(stage, 3)) * t2;      // L2
  Ciphertext s = top + mid;                         // L3
  return s + t * coeff(stage, 1);                   // c1 t aligned L1 -> L3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext X3 = x * coeff(3, 3);
  Ciphertext X5 = x * coeff(3, 5);
  Ciphertext X7 = x * coeff(3, 7);
  Plaintext half("half");

  Ciphertext t = stage_base(x, 1);
  t = stage_base(t, 2);

  Ciphertext u = square(t);                  // L7
  Ciphertext u2 = square(u);                 // L8
  Ciphertext B = t * coeff(3, 1) + half;     // L7
  Ciphertext A0 = x * B;                     // L8
  Ciphertext A3 = (X3 * t) * u;              // L8
  Ciphertext H = (X7 * t) * u + X5 * t;      // L8
  Ciphertext high = H * u2;                  // L9
  Ciphertext y = (high + A3) + A0;
  y.set_output("y");
}

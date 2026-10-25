// Odd degree-7 evaluation per stage, p(t) = low + high * t^4 with
//   low  = t * (c3 * t^2 + c1)          (depth 3, 1 ct-pt + 1 ct-ct beyond u)
//   high = c5 * t + (c7 * t) * t^2      (depth 2)
// Per stage: 5 ct-ct multiplies (u, v, low product, (c7 t)u, high*v), 3 ct-pt multiplies.
// Depth per stage is 3, so 3 stages + final x*h give 10 levels.

Plaintext coeff(int stage, int k)
{
  return Plaintext("s" + to_string(stage) + "c" + to_string(k));
}

Ciphertext stage_eval(const Ciphertext &t, int stage)
{
  Ciphertext u = t * t;   // t^2
  Ciphertext v = u * u;   // t^4

  Ciphertext low = t * (u * coeff(stage, 3) + coeff(stage, 1));
  Ciphertext high = t * coeff(stage, 5) + (t * coeff(stage, 7)) * u;

  return low + high * v;
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

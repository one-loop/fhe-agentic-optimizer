// Stage polynomial as in attempt 1: p = t^4*A + B,
// A = c5*t + (c7*t)*t^2, B = (c3*t)*t^2 + c1*t  (depth 3, 5 ct-ct multiplies).
// Final: y = x*t3 + half*x, so the plaintext-scaled term is computed at a shallow
// level and only one ciphertext-ciphertext multiply (x*t3) sits on the last level.

Ciphertext odd_stage(const Ciphertext &t, int s)
{
  Plaintext c1("s" + to_string(s) + "c1");
  Plaintext c3("s" + to_string(s) + "c3");
  Plaintext c5("s" + to_string(s) + "c5");
  Plaintext c7("s" + to_string(s) + "c7");

  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext A = t * c5 + (t * c7) * t2;
  Ciphertext B = (t * c3) * t2 + t * c1;
  return t4 * A + B;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = odd_stage(t, stage);
  Plaintext half("half");
  Ciphertext hx = x * half;
  Ciphertext y = x * t + hx;
  y.set_output("y");
}

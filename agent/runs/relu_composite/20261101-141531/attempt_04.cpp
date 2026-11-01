Ciphertext eval_stage(const Ciphertext &t, int stage)
{
  auto c = [&](int k) { return Plaintext("s" + to_string(stage) + "c" + to_string(k)); };
  Ciphertext u = t * t;                                  // L+1
  Ciphertext t4 = u * u;                                 // L+2
  Ciphertext high = t * c(5) + (t * c(7)) * u;           // L+2
  Ciphertext rest = (t * c(3)) * u + t4 * high;          // L+3 (L+2 term aligned)
  return t * c(1) + rest;                                // L+1 term aligned to L+3
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 2; ++stage)
    t = eval_stage(t, stage);

  // Stage 3 fused with the final multiplication by x:
  // y = x*(t*Q(u) + 1/2) = ((z+z)*t)*Q(u) + z, z = x/2, u = t^2.
  Plaintext c1("s3c1"), c3("s3c3"), c5("s3c5"), c7("s3c7");
  Plaintext half("half");
  Ciphertext z = x * half;                    // level 1
  Ciphertext u = t * t;                       // level 7
  Ciphertext u2 = u * u;                      // level 8
  Ciphertext hi = u2 * (u * c7 + c5);         // level 9
  Ciphertext q = (u * c3 + c1) + hi;          // level 9
  Ciphertext xt = (z + z) * t;                // level 7
  Ciphertext y = xt * q + z;                  // level 10
  y.set_output("y");
}

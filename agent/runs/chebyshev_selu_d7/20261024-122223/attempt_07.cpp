void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext z_square = square(z);
  t[2] = z_square + z_square - one;
  for (int k = 3; k <= 7; ++k)
  {
    Ciphertext z_t = z * t[k - 1];
    t[k] = z_t + z_t - t[k - 2];
  }

  Ciphertext r = t[1] * c1;
  r = r + t[2] * c2;
  r = r + t[3] * c3;
  r = r + t[4] * c4;
  r = r + t[5] * c5;
  r = r + t[6] * c6;
  r = r + t[7] * c7;
  r = r + c0;
  r.set_output("y");
}

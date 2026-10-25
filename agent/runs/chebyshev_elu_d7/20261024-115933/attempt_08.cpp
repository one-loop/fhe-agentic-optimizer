void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext z_square = z * z;
  t[2] = z_square + z_square - one;
  for (int k = 3; k <= 7; ++k)
  {
    Ciphertext z_t = z * t[k - 1];
    t[k] = z_t + z_t - t[k - 2];
  }
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = t[1] * c1;
  result = result + t[2] * c2;
  result = result + t[3] * c3;
  result = result + t[4] * c4;
  result = result + t[5] * c5;
  result = result + t[6] * c6;
  result = result + t[7] * c7;
  result = result + c0;
  result.set_output("y");
}

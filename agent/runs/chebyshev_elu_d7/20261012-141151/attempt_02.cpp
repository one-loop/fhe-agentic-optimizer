void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext z2 = z * z;
  t[2] = z2 + z2 - one;
  for (int k = 3; k <= 7; ++k)
  {
    Ciphertext zt = z * t[k - 1];
    t[k] = zt + zt - t[k - 2];
  }

  Plaintext c1("c1");
  Ciphertext result = t[1] * c1;
  Plaintext c2("c2");
  result = result + t[2] * c2;
  Plaintext c3("c3");
  result = result + t[3] * c3;
  Plaintext c4("c4");
  result = result + t[4] * c4;
  Plaintext c5("c5");
  result = result + t[5] * c5;
  Plaintext c6("c6");
  result = result + t[6] * c6;
  Plaintext c7("c7");
  result = result + t[7] * c7;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}

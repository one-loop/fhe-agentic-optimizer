void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext s = square(z);
  t[2] = s + s - one;
  Ciphertext w3 = z * t[2];
  t[3] = w3 + w3 - z;
  Ciphertext s4 = square(t[2]);
  t[4] = s4 + s4 - one;
  Ciphertext w5 = t[2] * t[3];
  t[5] = w5 + w5 - z;
  Ciphertext s6 = square(t[3]);
  t[6] = s6 + s6 - one;
  Ciphertext w7 = t[3] * t[4];
  t[7] = w7 + w7 - z;
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

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext zc4 = z * c4;
  Ciphertext t2c5 = t2 * c5;
  Ciphertext t4c7 = t4 * c7;
  Ciphertext B = zc4 + zc4;
  B = B + t2c5 + t2c5;
  B = B + t4c7 + t4c7;
  B = B + c3;
  Ciphertext main = t3 * B;

  Ciphertext t2c6 = t2 * c6;
  Ciphertext p6 = t2c6 * t4;
  p6 = p6 + p6;

  Ciphertext lowz = z * c1;
  lowz = lowz - z * c5;
  lowz = lowz - z * c7;
  Ciphertext lowt = t2 * c2;
  lowt = lowt - t2 * c4;
  lowt = lowt - t2c6;
  Ciphertext low = lowz + lowt;
  low = low + c0;

  Ciphertext result = low + p6;
  result = result + main;
  result.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext m3 = w * c3;
  Ciphertext m5 = w * c5;
  Ciphertext m5x2 = m5 + m5;
  Ciphertext m5x4 = m5x2 + m5x2;
  Ciphertext m5x5 = m5x4 + m5;
  Ciphertext r = (m3 - m5x5) + w * m5;
  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc3x3 = (zc3 + zc3) + zc3;
  Ciphertext zc5x2 = zc5 + zc5;
  Ciphertext zc5x5 = (zc5x2 + zc5x2) + zc5;
  Ciphertext K = (zc1 - zc3x3) + zc5x5;
  Ciphertext y = K + z * r;
  y = y + c0;
  y.set_output("y");
}

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
  Ciphertext m3u = u * c3;
  Ciphertext m5u = u * c5;
  Ciphertext m3a = m3u + m3u;
  Ciphertext m3 = m3a + m3a;
  Ciphertext m5a = m5u + m5u;
  Ciphertext m5b = m5a + m5a;
  Ciphertext m5c = m5b + m5b;
  Ciphertext m5d = m5c + m5c;
  Ciphertext m5x20 = m5d + m5b;
  Ciphertext prod = u * m5d;
  Ciphertext r = (m3 - m5x20) + prod;
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

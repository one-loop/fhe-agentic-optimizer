void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // Chebyshev basis
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // low part: c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3 + c4 T4
  Ciphertext low = z * c1;
  Ciphertext a2 = T2 * c2;
  Ciphertext a3 = T3 * c3;
  Ciphertext a4 = T4 * c4;
  low = low + a2;
  low = low + a3;
  low = low + a4;
  low = low + c0;
  Ciphertext s1 = z * c7;
  Ciphertext s2 = T2 * c6;
  Ciphertext s3 = T3 * c5;
  low = low - s1;
  low = low - s2;
  low = low - s3;

  // high part: q = c5 T1 + c6 T2 + c7 T3, contributes 2 T4 q
  Ciphertext q1 = z * c5;
  Ciphertext q2 = T2 * c6;
  Ciphertext q3 = T3 * c7;
  Ciphertext q = q1 + q2;
  q = q + q3;
  Ciphertext P = T4 * q;
  Ciphertext P2 = P + P;

  Ciphertext result = low + P2;
  result.set_output("y");
}

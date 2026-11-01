void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T3 = 2 z T2 - z
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  // T4 = 2 T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // A = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
  Plaintext c1("c1");
  Ciphertext a1 = z * c1;
  Plaintext c2("c2");
  Ciphertext a2 = T2 * c2;
  Plaintext c3("c3");
  Ciphertext a3 = T3 * c3;
  Plaintext c5("c5");
  Ciphertext a5 = T3 * c5;
  Plaintext c6("c6");
  Ciphertext a6 = T2 * c6;
  Plaintext c7("c7");
  Ciphertext a7 = z * c7;
  Ciphertext A = a1 + a2;
  A = A + a3;
  A = A - a5;
  A = A - a6;
  A = A - a7;

  // B = 2 (c5 T1 + c6 T2 + c7 T3) + c4
  Ciphertext u1 = z * c5;
  Ciphertext u2 = T2 * c6;
  Ciphertext u3 = T3 * c7;
  Ciphertext Bp = u1 + u2;
  Bp = Bp + u3;
  Ciphertext B = Bp + Bp;
  Plaintext c4("c4");
  B = B + c4;

  // p = A + T4 * B
  Ciphertext P = T4 * B;
  Ciphertext result = A + P;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}

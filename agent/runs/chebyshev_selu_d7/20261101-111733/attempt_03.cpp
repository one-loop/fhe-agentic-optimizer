void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext T1 = x * alpha;
  Ciphertext zz = T1 * T1;
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext n = T1 * c7;
  Ciphertext a5 = T1 * c5;
  Ciphertext m = T1 * c3 - a5;
  Ciphertext l1 = T1 * c1 - n;

  Ciphertext a6 = T2 * c6;
  Ciphertext l2 = T2 * c2 - a6;

  Ciphertext nu = n * u;
  Ciphertext mu = m * u;

  Ciphertext q = nu + a6 + a5;
  Ciphertext inner = q + q + c4;
  Ciphertext high = T4 * inner;

  Ciphertext result = high + mu + l2 + l1 + c0;
  result.set_output("y");
}

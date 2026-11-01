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
  Ciphertext T3 = T1 * u;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext a5 = T1 * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext q = a7 + a6 + a5;
  Ciphertext inner = q + q + c4;
  Ciphertext high = T4 * inner;

  Ciphertext l1 = T1 * c1 - T1 * c7;
  Ciphertext l2 = T2 * c2 - a6;
  Ciphertext l3 = T3 * c3 - T3 * c5;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext result = high + low + c0;
  result.set_output("y");
}

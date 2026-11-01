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
  Ciphertext zz = square(T1);
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext T3 = T1 * u;
  Ciphertext t22 = square(T2);

  // level-2 terms
  Ciphertext a5 = T1 * c5;
  Ciphertext l1 = T1 * c1 - T1 * c7;

  // level-3 terms, L2 terms folded in once
  Ciphertext a6 = T2 * c6;
  Ciphertext q23 = a6 + a5;
  Ciphertext low23 = T2 * c2 - a6 + l1;

  // level-4 terms
  Ciphertext a7 = T3 * c7;
  Ciphertext l3 = T3 * c3 - T3 * c5;
  Ciphertext q = a7 + q23;
  Ciphertext inner = q + q + c4;
  Ciphertext low = l3 + low23;

  Ciphertext P = t22 * inner;
  Ciphertext rest = low - inner;
  Ciphertext result = P + P + rest;
  result = result + c0;
  result.set_output("y");
}

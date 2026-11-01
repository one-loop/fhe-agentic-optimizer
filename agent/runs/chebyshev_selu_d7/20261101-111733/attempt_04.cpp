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

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext w = z7 + z7 + c6;
  Ciphertext d = z5 - z7;
  Ciphertext g = d + d + c4;
  Ciphertext combo = z3 - d;

  Ciphertext T2w = T2 * w;
  Ciphertext inner = T2w + T2w + g;
  Ciphertext high = T4 * inner;

  Ciphertext mid = combo * u - T2w;
  Ciphertext low = T2 * c2 + z1;

  Ciphertext result = high + mid;
  result = result + low;
  result = result + c0;
  result.set_output("y");
}

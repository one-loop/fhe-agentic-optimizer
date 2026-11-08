void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext uw4 = u4 * w;
  Ciphertext r = (v - u5) + uw4;
  Ciphertext r2 = r + r;
  Ciphertext r4 = r2 + r2;
  Ciphertext m4 = w * r4;

  Ciphertext v2 = v + v;
  Ciphertext v3 = v2 + v;
  Ciphertext s0 = s + c0;
  Ciphertext low = (s0 - v3) + u5;

  Ciphertext result = low + m4;
  result.set_output("y");
}

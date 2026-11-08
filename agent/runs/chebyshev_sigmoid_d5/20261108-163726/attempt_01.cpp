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

  Ciphertext uw = u * w;
  Ciphertext uw2 = uw + uw;
  Ciphertext uw4 = uw2 + uw2;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext r = (v - u5) + uw4;

  Ciphertext m = w * r;
  Ciphertext m2 = m + m;
  Ciphertext m4 = m2 + m2;

  Ciphertext v2 = v + v;
  Ciphertext v3 = v2 + v;
  Ciphertext low = (s - v3) + u5;

  Ciphertext result = low + m4;
  result = result + c0;
  result.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext ua = u * c6;
  Ciphertext q = ua + ua;
  q = q - c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext w = t4 * v;
  Ciphertext ub = u * c2;
  Ciphertext t2c2 = ub + ub;
  t2c2 = t2c2 - c2;
  Ciphertext result = z * c1;
  result = result + t2c2;
  result = result - q;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}

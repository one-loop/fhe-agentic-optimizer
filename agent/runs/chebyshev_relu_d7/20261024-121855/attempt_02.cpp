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
  Ciphertext w = t4 * t2;
  Ciphertext sc = s * c4;
  Ciphertext wc = w * c6;
  Ciphertext uc = u * c2;
  Ciphertext t2c = t2 * c6;
  Ciphertext result = z * c1;
  result = result + (uc + uc);
  result = result + (sc + sc);
  result = result + (wc + wc);
  result = result - t2c;
  result = result - c4;
  result = result - c2;
  result = result + c0;
  result.set_output("y");
}

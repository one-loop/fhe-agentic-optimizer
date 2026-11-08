void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext term5 = p5 + p5 - z * c5;
  Ciphertext p7 = (t4 * c7) * t3;
  Ciphertext term7 = p7 + p7 - z * c7;
  Ciphertext low = z * c1 + (z * c3) * u;
  Ciphertext result = low + term5;
  result = result + term7;
  result = result + c0;
  result.set_output("y");
}

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
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext s = t4 + t4 - one;
  Ciphertext p4 = w * c4;
  Ciphertext bracket = p4 + p4 + s * c6;
  bracket = bracket + c2;
  Ciphertext result = w * bracket;
  result = result + z * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}

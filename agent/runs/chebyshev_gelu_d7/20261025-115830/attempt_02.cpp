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
  Ciphertext t2 = u + u - one;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext a = t2 * c6;
  Ciphertext b = t4 + t4 - one;
  Ciphertext t6c = a * b;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6c;
  result = result + c0;
  result.set_output("y");
}

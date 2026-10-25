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
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext u = t2 * c6;
  Ciphertext v = u + u + c4;
  Ciphertext m = t4 * v;
  Ciphertext term2 = t2 * c2;
  Ciphertext term1 = z * c1;
  Ciphertext low = term2 - u;
  Ciphertext result = term1 + low;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}

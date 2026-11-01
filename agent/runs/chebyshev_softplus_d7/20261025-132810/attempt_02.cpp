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
  Ciphertext s = square(t2);
  Ciphertext w = t2 * c6;
  Ciphertext q = w * s;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext a = s * c4;
  Ciphertext a2 = a + a;
  Ciphertext b = t2 * c2;
  Ciphertext w3 = w + w + w;
  Ciphertext result = q4 + a2;
  result = result + b;
  result = result - w3;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext w = (z * c5) * t4;
  Ciphertext v = (t3 * c7) * t4;
  Ciphertext result = v + v;
  result = result + (w + w);
  result = result + t3 * c3;
  result = result - t3 * c5;
  result = result + z * c1;
  result = result - z * c7;
  result = result + c0;
  result.set_output("y");
}

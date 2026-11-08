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
  Ciphertext s2 = t2 * t2;
  Ciphertext a7 = t2 * c7;
  Ciphertext a5 = t2 * c5;
  Ciphertext a3 = t2 * c3;
  Ciphertext g = a7 + a7;
  g = g + c5;
  g = g - c7;
  Ciphertext h = g * s2;
  h = h + h;
  h = h + h;
  Ciphertext lin = a3 - a5 - a7 - a7;
  lin = lin + c7;
  lin = lin + lin;
  lin = lin - c3;
  Ciphertext q = h + lin;
  q = q + c1;
  q = q - c5;
  q = q - c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}

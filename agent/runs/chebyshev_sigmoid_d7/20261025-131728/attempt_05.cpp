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
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext v = w + w - one;
  Ciphertext ww = w * w;
  Ciphertext a = ww * c7;
  Ciphertext q = w * c5 + a;
  q = q + a;
  q = q - c7;
  Ciphertext r = q + q + c3;
  Ciphertext p = r * v;
  p = p + c1;
  p = p - c5;
  p = p - c7;
  Ciphertext result = z * p;
  result = result + c0;
  result.set_output("y");
}

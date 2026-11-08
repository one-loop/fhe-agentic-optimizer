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
  Ciphertext m = t2 * c7;
  m = m + m;
  m = m + m;
  m = m + c5;
  m = m + c5;
  Ciphertext inner = t2 * m;
  inner = inner + c3;
  inner = inner - c7;
  inner = inner - c7;
  Ciphertext q = u * inner;
  q = q + c1;
  q = q - c5;
  q = q - c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}

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
  Ciphertext s = square(w);
  Ciphertext q = s * c6;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext inner = q4 - c6;
  inner = inner - c6;
  inner = inner - c6;
  inner = inner + c2;
  Ciphertext f = w * inner;
  Ciphertext p = s * c4;
  Ciphertext r = p + p;
  r = r - c4;
  Ciphertext result = z * c1;
  result = result + r;
  result = result + f;
  result = result + c0;
  result.set_output("y");
}

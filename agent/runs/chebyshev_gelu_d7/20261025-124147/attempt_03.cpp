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
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;
  Ciphertext s = w * w;
  Ciphertext t = s + s - one;
  Ciphertext q = t + t - one;
  Ciphertext b = q * c6;
  Ciphertext a = w * c4;
  Ciphertext a2 = a + a;
  Ciphertext a3 = a2 + c2;
  Ciphertext inner = b + a3;
  Ciphertext p = w * inner - c4;
  Ciphertext r = p + z * c1;
  r = r + c0;
  r.set_output("y");
}

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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 + t4 - one;
  Ciphertext vc4 = v * c4;
  Ciphertext inner = w * c6 + (vc4 + vc4) + c2;
  Ciphertext result = v * inner - c4;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}

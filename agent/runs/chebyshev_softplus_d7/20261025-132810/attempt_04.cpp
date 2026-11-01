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
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext q = inner * s;
  Ciphertext b = t2 * c2;
  Ciphertext w3 = w2 + w;
  Ciphertext result = q + b;
  result = result - w3;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}

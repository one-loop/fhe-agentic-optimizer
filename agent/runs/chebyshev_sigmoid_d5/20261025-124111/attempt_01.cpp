void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext a = t2 + t2 - one;
  Ciphertext q = square(t2);
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext b = q4 - t2 - t2 - one;
  Ciphertext inner = a * c3 + b * c5;
  Ciphertext s = inner + c1;
  Ciphertext result = s * z + c0;
  result.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext z3 = z * w;
  Ciphertext z5 = w * z3;
  Ciphertext z7 = w * z5;
  Ciphertext r = z * c1;
  r = r + z3 * c3;
  r = r + z5 * c5;
  r = r + z7 * c7;
  r.set_output("y");
}

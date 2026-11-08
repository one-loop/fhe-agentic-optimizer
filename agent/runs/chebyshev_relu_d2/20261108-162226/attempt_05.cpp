void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext s2 = s + s;
  Ciphertext q = s2 * c2;
  Ciphertext t = z * c1;
  Ciphertext result = q + t;
  result = result + c0;
  result = result - c2;
  result.set_output("y");
}

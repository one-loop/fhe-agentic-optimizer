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
  Ciphertext z2 = square(z);
  Ciphertext t2 = (z2 + z2) - one;   // 2z^2 - 1
  Ciphertext m = t2 + t2;            // 4z^2 - 2
  Ciphertext n = m - one;            // 4z^2 - 3
  Ciphertext t5 = m * n - one;       // 16z^4 - 20z^2 + 5

  Ciphertext B = (n * c3 + t5 * c5) + c1;
  Ciphertext result = z * B + c0;
  result.set_output("y");
}

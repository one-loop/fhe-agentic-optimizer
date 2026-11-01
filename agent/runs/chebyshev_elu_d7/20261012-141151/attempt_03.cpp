void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;

  Ciphertext z2 = z * z;
  Ciphertext z3 = z * z2;
  Ciphertext z4 = z2 * z2;

  // Block A = c1 z + c2 z^2 + c3 z^3 (constant c0 added at the end)
  Plaintext c1("c1");
  Ciphertext a1 = z * c1;
  Plaintext c2("c2");
  Ciphertext a2 = z2 * c2;
  Plaintext c3("c3");
  Ciphertext a3 = z3 * c3;
  Ciphertext A = a1 + a2;
  A = A + a3;

  // Block B = c4 + c5 z + c6 z^2 + c7 z^3
  Plaintext c5("c5");
  Ciphertext b1 = z * c5;
  Plaintext c6("c6");
  Ciphertext b2 = z2 * c6;
  Plaintext c7("c7");
  Ciphertext b3 = z3 * c7;
  Ciphertext B = b1 + b2;
  B = B + b3;
  Plaintext c4("c4");
  B = B + c4;

  // p = A + z^4 * B
  Ciphertext P = z4 * B;
  Ciphertext result = A + P;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}

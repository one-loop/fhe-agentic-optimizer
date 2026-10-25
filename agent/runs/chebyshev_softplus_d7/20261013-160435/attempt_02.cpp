// Chebyshev softplus degree 7 on [-4,4], using the doubling identities
// T2 = 2z^2 - 1, T4 = 2 T2^2 - 1, T6 = 2 T2 T4 - T2 (z = x/4).
// Only 3 ciphertext multiplications for the powers, then 4 plaintext multiplications.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z_square = z * z;
  Ciphertext t2 = z_square + z_square - one;

  // T4 = 2 T2^2 - 1
  Ciphertext t2_square = t2 * t2;
  Ciphertext t4 = t2_square + t2_square - one;

  // T6 = 2 T2 T4 - T2
  Ciphertext t24 = t2 * t4;
  Ciphertext t6 = t24 + t24 - t2;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;
  Plaintext c2("c2");
  Ciphertext term2 = t2 * c2;
  Plaintext c4("c4");
  Ciphertext term4 = t4 * c4;
  Plaintext c6("c6");
  Ciphertext term6 = t6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}

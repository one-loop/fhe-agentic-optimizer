// y = c0 + c1 z + c3 A + c5 B,  z = x/4
// A = 4 z^3 - 3 z,  B = 16 z^5 - 20 z^3 + 5 z   (built with additions only)

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext z3 = z2 * z;
  Ciphertext z5 = z3 * z2;

  // 3z and 5z, 4z
  Ciphertext z2x = z + z;
  Ciphertext z3x = z2x + z;
  Ciphertext z4x = z2x + z2x;
  Ciphertext z5x = z4x + z;

  // 4 z^3, 16 z^3, 20 z^3
  Ciphertext z3x4 = z3 + z3;
  z3x4 = z3x4 + z3x4;
  Ciphertext z3x8 = z3x4 + z3x4;
  Ciphertext z3x16 = z3x8 + z3x8;
  Ciphertext z3x20 = z3x16 + z3x4;

  // 16 z^5
  Ciphertext z5x2 = z5 + z5;
  Ciphertext z5x4 = z5x2 + z5x2;
  Ciphertext z5x8 = z5x4 + z5x4;
  Ciphertext z5x16 = z5x8 + z5x8;

  // A = 4z^3 - 3z,  B = 16z^5 - 20z^3 + 5z
  Ciphertext A = z3x4 - z3x;
  Ciphertext B = z5x16 - z3x20;
  B = B + z5x;

  Ciphertext result = z * c1;
  result = result + A * c3;
  result = result + B * c5;
  result = result + c0;
  result.set_output("y");
}

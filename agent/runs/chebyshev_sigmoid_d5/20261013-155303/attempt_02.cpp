// y = c0 + c1 z + c3 (4z^3 - 3z) + c5 (16z^5 - 20z^3 + 5z),  z = x/4
// Shared structure: z*(c1 - 3c3 + 5c5) + z^3*(4c3 - 20c5) + z^5*(16c5)

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

  // Linear-in-z part: (c1 - 3c3 + 5c5) * z, built from three pt muls on z.
  Ciphertext p1 = z * c1;
  Ciphertext p3 = z * c3;
  Ciphertext p5 = z * c5;
  Ciphertext p3x3 = p3 + p3;
  p3x3 = p3x3 + p3;
  Ciphertext p5x4 = p5 + p5;
  p5x4 = p5x4 + p5x4;
  Ciphertext p5x5 = p5 + p5x4;
  Ciphertext lin = p1 + p5x5;
  lin = lin - p3x3;

  // z^3 part: (4c3 - 20c5) * z^3.
  Ciphertext q3 = z3 * c3;
  Ciphertext q5 = z3 * c5;
  Ciphertext a4 = q3 + q3;
  a4 = a4 + a4;
  Ciphertext q4 = q5 + q5;
  q4 = q4 + q4;
  Ciphertext q16 = q4 + q4;
  q16 = q16 + q16;
  Ciphertext t20 = q16 + q4;
  Ciphertext cub = a4 - t20;

  // z^5 part: 16 c5 * z^5.
  Ciphertext r5 = z5 * c5;
  Ciphertext b16 = r5 + r5;
  b16 = b16 + b16;
  b16 = b16 + b16;
  b16 = b16 + b16;

  Ciphertext result = lin + cub;
  result = result + b16;
  result = result + c0;
  result.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  // pad: consume n levels by multiplying with the plaintext 1
  auto pad = [&](Ciphertext c, int n) {
    for (int i = 0; i < n; ++i)
      c = c * one;
    return c;
  };

  // z = x/4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;

  // T4 = 2 T2^2 - 1  (depth 3)
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  // T5 = 2 T2 T3 - z  (depth 4)
  Ciphertext t2t3 = t2 * t3;
  Ciphertext t5 = t2t3 + t2t3 - z;

  // T6 = 2 T3^2 - 1  (depth 4)
  Ciphertext t3sq = t3 * t3;
  Ciphertext t6 = t3sq + t3sq - one;

  // T7 = 2 T3 T4 - z  (depth 4)
  Ciphertext t3t4 = t3 * t4;
  Ciphertext t7 = t3t4 + t3t4 - z;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // terms: levels 2,3,4,4,5,(6 after pad),5
  Ciphertext term1 = z * c1;
  Ciphertext term2 = t2 * c2;
  Ciphertext term3 = t3 * c3;
  Ciphertext term4 = t4 * c4;
  Ciphertext term5 = t5 * c5;
  Ciphertext term6 = pad(t6, 1) * c6;
  Ciphertext term7 = t7 * c7;

  // tree sum: every addition joins operands at different levels
  Ciphertext C = term1 + term2;      // levels 2,3 -> 3
  Ciphertext A = term3 + term5;      // levels 4,5 -> 5
  Ciphertext B = term4 + term6;      // levels 4,6 -> 6
  Ciphertext D = A + B;              // levels 5,6 -> 6
  Ciphertext E = D + term7;          // levels 6,5 -> 6
  Ciphertext F = E + C;              // levels 6,3 -> 6
  Ciphertext result = F + c0;

  result.set_output("y");
}

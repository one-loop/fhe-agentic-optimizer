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

  Ciphertext r = z * c1;
  Ciphertext q = z * c3;
  Ciphertext p = z * c5;

  Ciphertext p2 = p + p;
  Ciphertext p4 = p2 + p2;
  Ciphertext p8 = p4 + p4;
  Ciphertext p16 = p8 + p8;
  Ciphertext p20 = p16 + p4;
  Ciphertext p5 = p4 + p;

  Ciphertext q2 = q + q;
  Ciphertext q3 = q2 + q;
  Ciphertext q4 = q2 + q2;

  // linear part: (c1 - 3c3 + 5c5) z
  Ciphertext lin = r - q3 + p5;
  // inner: (4c3 - 20c5) z + 16 c5 z^3
  Ciphertext inner = (q4 - p20) + z2 * p16;
  Ciphertext result = lin + z2 * inner;
  result = result + c0;
  result.set_output("y");
}

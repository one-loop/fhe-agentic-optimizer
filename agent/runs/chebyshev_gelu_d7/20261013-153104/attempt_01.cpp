// Chebyshev gelu degree 7: only the even-index basis terms needed (T1, T2, T4, T6)
// are built, using T_{m+n} = 2 T_m T_n - T_{|m-n|}. This uses 3 ciphertext-ciphertext
// multiplications instead of 5, and the doubling is done by self-addition.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;                 // z in [-1,1], depth 1

  Ciphertext z2 = z * z;                    // depth 2
  Ciphertext T2 = z2 + z2 - one;            // T2 = 2z^2 - 1

  Ciphertext T2sq = T2 * T2;                // depth 3
  Ciphertext T4 = T2sq + T2sq - one;        // T4 = 2 T2^2 - 1

  Ciphertext T42 = T4 * T2;                 // depth 4
  Ciphertext T6 = T42 + T42 - T2;           // T6 = 2 T4 T2 - T2

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext term1 = z * c1;
  Ciphertext term2 = T2 * c2;
  Ciphertext term4 = T4 * c4;
  Ciphertext term6 = T6 * c6;

  Ciphertext result = term6 + term4;
  result = result + term2;
  result = result + term1;
  result = result + c0;
  result.set_output("y");
}

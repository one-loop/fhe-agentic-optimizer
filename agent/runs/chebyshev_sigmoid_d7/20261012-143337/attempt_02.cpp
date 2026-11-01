// Chebyshev sigmoid degree 7 on [-4,4].
// T_{m+n} = 2 T_m T_n - T_{|m-n|}; c5*T5 is formed as 2*((c5*T2)*T3) - c5*z
// so that its level is 4 instead of 5, avoiding same-level scale mismatches.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;            // T1, depth 1

  Ciphertext zz = z * z;               // depth 2
  Ciphertext T2 = zz + zz - one;       // T2

  Ciphertext zT2 = z * T2;             // depth 3
  Ciphertext T3 = zT2 + zT2 - z;       // T3

  Ciphertext T2s = T2 * T2;            // depth 3
  Ciphertext T4 = T2s + T2s - one;     // T4

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  // c1*T1 and c3*T3
  Ciphertext term1 = z * c1;
  Ciphertext term3 = T3 * c3;

  // c5*T5 = 2*((c5*T2)*T3) - c5*z   (level 4)
  Ciphertext c5T2 = T2 * c5;           // depth 3
  Ciphertext P5 = c5T2 * T3;           // depth 4
  Ciphertext c5z = z * c5;             // depth 2
  Ciphertext term5 = P5 + P5 - c5z;

  // c7*T7 = 2*T3*(T4*c7) - c7*z   (level 5)
  Ciphertext T4c7 = T4 * c7;           // depth 4
  Ciphertext P7 = T3 * T4c7;           // depth 5
  Ciphertext c7z = z * c7;             // depth 2
  Ciphertext term7 = P7 + P7 - c7z;

  Ciphertext result = term1 + term3;
  result = result + term7;
  result = result + term5;
  result = result + c0;
  result.set_output("y");
}

// Chebyshev hardshrink degree 7 on [-4,4].
// Odd terms only: T2 = 2z^2-1, T3 = 2z*T2 - z, T5 = 2*T2*T3 - z, T7 = 2*T2*T5 - T3.
// Four ciphertext-ciphertext multiplies, depth 6. Sum ordered deepest-first.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;

  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;

  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  Ciphertext T2T3 = T2 * T3;
  Ciphertext T5 = T2T3 + T2T3 - z;

  Ciphertext T2T5 = T2 * T5;
  Ciphertext T7 = T2T5 + T2T5 - T3;

  Ciphertext t7 = T7 * c7;
  Ciphertext t5 = T5 * c5;
  Ciphertext t3 = T3 * c3;
  Ciphertext t1 = z * c1;

  Ciphertext result = t7 + t5;
  result = result + t3;
  result = result + t1;
  result = result + c0;

  result.set_output("y");
}

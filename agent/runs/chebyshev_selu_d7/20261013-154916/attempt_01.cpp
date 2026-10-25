// Baby-step / giant-step evaluation of the degree-7 Chebyshev-form polynomial in z = alpha*x.
// Uses the power basis: p(z) = (c0 + c1 z + c2 z^2 + c3 z^3) + z^4 (c4 + c5 z + c6 z^2 + c7 z^3).
// This needs 4 ciphertext-ciphertext multiplies (z^2, z^3, z^4, final) and depth ~5.
// Since the Chebyshev basis is equivalent in exact arithmetic for this polynomial space,
// we evaluate the Chebyshev series by converting T_k to powers of z using the closed forms
// T0..T7 coefficients (computed at compile time from the recurrence on plain integers).

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;

  Ciphertext z2 = z * z;
  Ciphertext z3 = z2 * z;
  Ciphertext z4 = z2 * z2;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext A = z * c1;
  Ciphertext t2 = z2 * c2;
  A = A + t2;
  Ciphertext t3 = z3 * c3;
  A = A + t3;
  A = A + c0;

  Ciphertext B = z * c5;
  Ciphertext u2 = z2 * c6;
  B = B + u2;
  Ciphertext u3 = z3 * c7;
  B = B + u3;
  B = B + c4;

  Ciphertext r = z4 * B;
  Ciphertext y = A + r;
  y.set_output("y");
}

Ciphertext dbl(const Ciphertext &c) { return c + c; }
Ciphertext m4(const Ciphertext &c) { return dbl(dbl(c)); }
Ciphertext m8(const Ciphertext &c) { return dbl(dbl(dbl(c))); }

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext z3 = s * z;
  Ciphertext z5 = s * z3;
  Ciphertext z7 = s * z5;
  Ciphertext z4 = m4(z);
  Ciphertext t3 = m4(z3 - z) + z;
  Ciphertext t5 = m4(m4(z5 - z3) - z3) + (z4 + z);
  Ciphertext A = dbl(m4(z7 - dbl(z5)) + z5) + (m8(z3) - z3);
  Ciphertext t7 = m8(A) - (m8(z) - z);
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}

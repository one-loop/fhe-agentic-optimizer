void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext p3 = p + p + p;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext q5 = q4 + q;
  Ciphertext A = c1z - p3 + q5;
  Ciphertext pq = p - q5;
  Ciphertext inner4 = pq + u * q4;
  Ciphertext inner2 = inner4 + inner4;
  Ciphertext inner = inner2 + inner2;
  Ciphertext y = A + u * inner;
  y = y + c0;
  y.set_output("y");
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Ciphertext u4 = square(z2);
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext g = p - q;
  Ciphertext pq = g - q4;
  Ciphertext p2 = p + p;
  Ciphertext A = c1z - p2 - pq;
  Ciphertext A0 = A + c0;
  Ciphertext inner = pq + u4 * q;
  Ciphertext y = A0 + u4 * inner;
  y.set_output("y");
}

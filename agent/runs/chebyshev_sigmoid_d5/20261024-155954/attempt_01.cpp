void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext p3 = p + p + p;
  Ciphertext q5 = q + q + q + q + q;
  Ciphertext A = c1z - p3 + q5;
  Ciphertext q4 = q + q;
  q4 = q4 + q4;
  Ciphertext pq = p - q5;
  Ciphertext B = pq + pq;
  B = B + B;
  Ciphertext D = q4 + q4;
  D = D + D;
  Ciphertext inner = B + u * D;
  Ciphertext y = A + u * inner;
  y = y + c0;
  y.set_output("y");
}

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

  // z = x / 4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext u = z * z;
  Ciphertext T2 = u + u - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext p3 = z * T2;
  Ciphertext T3 = p3 + p3 - z;

  // T5 = 2 T2 T3 - z  (depth 4)
  Ciphertext p5 = T2 * T3;
  Ciphertext T5 = p5 + p5 - z;

  // T7 = 2 T2 T5 - T3  (depth 5)
  Ciphertext p7 = T2 * T5;
  Ciphertext T7 = p7 + p7 - T3;

  // y = c0 + c1 T1 + c3 T3 + c5 T5 + c7 T7
  Ciphertext y = z * c1;
  y = y + T3 * c3;
  y = y + T5 * c5;
  y = y + T7 * c7;
  y = y + c0;
  y.set_output("y");
}

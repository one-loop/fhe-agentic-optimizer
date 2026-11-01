void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext s = z2 + z2 - one;          // T2
  Ciphertext d2 = s + s;                 // 2 T2
  Ciphertext q = d2 - one;               // 2T2 - 1

  Ciphertext t = d2 * c7 + c5;           // 2 c7 T2 + c5
  Ciphertext r = s * t - c7;
  Ciphertext inner = r + r + c3;         // 4c7 T2^2 + 2c5 T2 - 2c7 + c3

  Ciphertext m = q * inner + c1 - c5 - c7;
  Ciphertext result = z * m;
  result.set_output("y");
}

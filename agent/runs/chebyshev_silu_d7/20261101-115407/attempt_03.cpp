void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext u = w + w - one;          // T2
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext inner = h + h;            // 4 c6 u + 2 c4
  Ciphertext mid = u * inner + c2 - c6 - c6 - c6;  // 4c6u^2 + 2c4u + c2 - 3c6
  Ciphertext e = u * mid;              // 4c6u^3 + 2c4u^2 + (c2-3c6)u
  Ciphertext lin = z * c1;
  Ciphertext result = e + lin + c0 - c4;
  result.set_output("y");
}

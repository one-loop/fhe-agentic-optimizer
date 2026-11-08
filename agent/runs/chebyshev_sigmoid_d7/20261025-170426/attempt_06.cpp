Ciphertext imul(const Ciphertext &c, int n)
{
  Ciphertext acc;
  Ciphertext cur = c;
  bool has = false;
  while (n > 0)
  {
    if (n & 1)
    {
      if (has)
        acc = acc + cur;
      else
      {
        acc = cur;
        has = true;
      }
    }
    n >>= 1;
    if (n > 0)
      cur = cur + cur;
  }
  return acc;
}

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

  // coefficient of z: c1 - 3c3 + 5c5 - 7c7
  Ciphertext g1 = (z * c1 + imul(z * c5, 5)) - (imul(z * c3, 3) + imul(z * c7, 7));
  // z^3: 4c3 - 20c5 + 56c7
  Ciphertext g3 = (imul(z3 * c3, 4) + imul(z3 * c7, 56)) - imul(z3 * c5, 20);
  // z^5: 16c5 - 112c7
  Ciphertext g5 = imul(z5 * c5, 16) - imul(z5 * c7, 112);
  // z^7: 64c7
  Ciphertext g7 = imul(z7 * c7, 64);

  Ciphertext result = g7 + g5;
  result = result + g3;
  result = result + g1;
  result = result + c0;
  result.set_output("y");
}

Ciphertext mulint(const Ciphertext &c, int n)
{
  Ciphertext acc;
  bool has = false;
  Ciphertext cur = c;
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
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext a = z * c1;
  Ciphertext b = z * c3;
  Ciphertext c = z * c5;
  Ciphertext d = z * c7;
  Ciphertext S = (a + mulint(c, 5)) - (mulint(b, 3) + mulint(d, 7));
  Ciphertext Q1 = (mulint(b, 4) + mulint(d, 56)) - mulint(c, 20);
  Ciphertext Q2 = mulint(c, 16) - mulint(d, 112);
  Ciphertext Q3 = mulint(d, 64);
  Ciphertext h = Q2 + w * Q3;
  h = Q1 + w * h;
  Ciphertext r = S + w * h;
  r.set_output("y");
}

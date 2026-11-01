Ciphertext mul_int(const Ciphertext &a, int n)
{
  Ciphertext r;
  bool h = false;
  Ciphertext p = a;
  while (n)
  {
    if (n & 1)
    {
      r = h ? r + p : p;
      h = true;
    }
    n >>= 1;
    if (n)
      p = p + p;
  }
  return r;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext v = u + u - one;
  Ciphertext v2 = square(v);
  Ciphertext v3 = v * v2;

  Ciphertext T4 = v2 + v2 - one;
  Ciphertext T6 = mul_int(v3, 4) - mul_int(v, 3);
  Ciphertext W3 = v + v - one;
  Ciphertext W5 = mul_int(v2, 4) - mul_int(v, 2) - one;
  Ciphertext W7 = mul_int(v3, 8) - mul_int(v2, 4) - mul_int(v, 4) + one;

  Ciphertext E = v * c2;
  E = E + T4 * c4;
  E = E + T6 * c6;

  Ciphertext O = W3 * c3;
  O = O + W5 * c5;
  O = O + W7 * c7;
  O = O + c1;

  Ciphertext result = E + z * O;
  result = result + c0;
  result.set_output("y");
}

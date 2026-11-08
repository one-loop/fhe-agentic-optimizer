void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext p0("table_0"), p1("table_1"), p2("table_2");

  Ciphertext t0 = indicator * p0;
  Ciphertext t1 = indicator * p1;
  Ciphertext t2 = indicator * p2;
  Ciphertext a = t0;
  Ciphertext b = t0 + t1;
  Ciphertext c = b + t2;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
  {
    a += a << static_cast<int>(s);
    b += b << static_cast<int>(s);
    c += c << static_cast<int>(s);
  }
  // a = S0, b = S0+S1, c = S0+S1+S2 (in every slot)
  Ciphertext d1 = b - a;
  Ciphertext d2 = c - b;
  Ciphertext output = a * first_slot;
  output += (d1 * first_slot) >> 1;
  output += (d2 * first_slot) >> 2;
  output.set_output("output");
}

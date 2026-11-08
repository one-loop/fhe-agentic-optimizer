void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext a = indicator * t0;
  Ciphertext b = indicator * t1;
  Ciphertext c = indicator * t2;
  for (size_t s = slots() >> 1; s > 0; s >>= 1)
  {
    a += a << static_cast<int>(s);
    b += b << static_cast<int>(s);
    c += c << static_cast<int>(s);
  }
  Ciphertext output = a * first_slot;
  output += (b * first_slot) >> 1;
  output += (c * first_slot) >> 2;
  output.set_output("output");
}

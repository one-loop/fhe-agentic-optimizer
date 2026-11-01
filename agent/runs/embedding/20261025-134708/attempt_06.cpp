void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> c;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    if (i == 0)
    {
      for (size_t s = slots() >> 1; s > 0; s >>= 1)
        r += r << static_cast<int>(s);
    }
    else if (i == 1)
    {
      for (size_t s = 1; s < slots(); s <<= 1)
        r += r << static_cast<int>(s);
    }
    else
    {
      for (size_t s = slots() >> 1; s > 0; s >>= 1)
        r += r >> static_cast<int>(s);
    }
    c.push_back(r);
  }

  Ciphertext output = c[0] * first_slot;
  output += (c[1] * first_slot) >> 1;
  output += (c[2] * first_slot) >> 2;
  output.set_output("output");
}

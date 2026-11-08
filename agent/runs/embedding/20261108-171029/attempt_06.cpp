void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> c;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    c.push_back(r);
  }

  Ciphertext m2 = c[2] * first_slot;
  Ciphertext m1 = c[1] * first_slot;
  Ciphertext t = (m2 >> 1) + m1;
  Ciphertext output = (t >> 1) + c[0] * first_slot;
  output.set_output("output");
}

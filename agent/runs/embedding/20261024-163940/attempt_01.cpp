void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext output;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    Ciphertext m = r * first_slot;
    if (i > 0)
      m = m >> i;
    if (i == 0)
      output = m;
    else
      output += m;
  }
  output.set_output("output");
}

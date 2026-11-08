void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  int half = static_cast<int>(slots() >> 1);
  Ciphertext output;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (int steps = 1; steps < half; steps <<= 1)
      r += r >> steps;
    Ciphertext a = r * first_slot;
    Ciphertext b = (r << half) * first_slot;
    Ciphertext m = a + b;
    if (i > 0)
      m = m >> i;
    if (i == 0)
      output = m;
    else
      output += m;
  }
  output.set_output("output");
}

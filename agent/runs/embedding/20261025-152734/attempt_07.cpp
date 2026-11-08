void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext acc;
  for (int i = 2; i >= 0; --i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r >> static_cast<int>(steps);
    Ciphertext m = r * first_slot;
    if (i == 2)
      acc = m;
    else
    {
      acc = acc >> 1;
      acc += m;
    }
  }
  acc.set_output("output");
}

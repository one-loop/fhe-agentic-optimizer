void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> m;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r >> static_cast<int>(steps);
    m.push_back(r * first_slot);
  }
  Ciphertext y = (m[0] << 1) + m[1];
  Ciphertext z = (y << 1) + m[2];
  Ciphertext output = (z >> 2);
  output.set_output("output");
}

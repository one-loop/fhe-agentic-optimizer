void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Plaintext e1 = first_slot >> 1;
  Plaintext e2 = first_slot >> 2;
  Ciphertext output = coordinates[0] * first_slot;
  output += coordinates[1] * e1;
  output += coordinates[2] * e2;
  output.set_output("output");
}

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

  Ciphertext m2 = coordinates[2] * first_slot;
  Ciphertext m1 = coordinates[1] * first_slot;
  Ciphertext m0 = coordinates[0] * first_slot;
  Ciphertext t = m1 + (m2 >> 1);
  Ciphertext output = m0 + (t >> 1);
  output.set_output("output");
}

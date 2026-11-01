void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  // y1 = y2 numerically (first_slot is idempotent), so w is zero.
  Ciphertext y1 = indicator * first_slot;
  Ciphertext y2 = y1 * first_slot;
  Ciphertext w = y2 - y1;
  for (int k = 0; k < 6; ++k)
    w = w * first_slot;            // still zero, now at level 8
  Ciphertext ind = indicator + w;  // indicator, dragged to level 8

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = ind * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}

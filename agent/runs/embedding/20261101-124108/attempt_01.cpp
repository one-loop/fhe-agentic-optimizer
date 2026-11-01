void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> prods;
  for (const auto &column : columns)
    prods.push_back(indicator * column);

  // Deep zero-sum term: y at level 9, w = y - rot(y) has total sum 0.
  Ciphertext y = prods[0] * first_slot;
  for (int k = 0; k < 7; ++k)
    y = y * first_slot;
  Ciphertext w = y - (y << 1);

  vector<Ciphertext> coordinates;
  for (auto &p : prods)
  {
    Ciphertext r = p + w;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}

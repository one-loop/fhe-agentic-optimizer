void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  // Unit-vector masks at slots 0, 1, 2 (plaintext rotations, no ciphertext cost).
  Plaintext e0 = first_slot;
  Plaintext e1 = first_slot >> 1;
  Plaintext e2 = first_slot >> 2;
  vector<Plaintext> masks = {e0, e1, e2};

  Ciphertext output = indicator * columns[0];
  Ciphertext r0 = indicator * columns[0];
  Ciphertext r1 = indicator * columns[1];
  Ciphertext r2 = indicator * columns[2];

  vector<Ciphertext> coordinates = {r0, r1, r2};
  for (auto &r : coordinates)
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);

  Ciphertext out = coordinates[0] * masks[0];
  for (size_t i = 1; i < coordinates.size(); ++i)
    out += coordinates[i] * masks[i];
  out.set_output("output");
}

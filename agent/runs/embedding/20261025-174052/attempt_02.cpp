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
  // m2: T2 at slot 0
  Ciphertext m2 = c[2] * first_slot;
  // t1: T1 everywhere in slots >= 0, plus T2 at slot 1 (T1 + T2 at slot 1)
  Ciphertext m1 = c[1] * first_slot;
  Ciphertext inner = m1 + (m2 >> 1);
  Ciphertext m0 = c[0] * first_slot;
  Ciphertext output = m0 + (inner >> 1);
  output.set_output("output");
}

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

  Ciphertext m0 = c[0] * first_slot;
  Ciphertext m1 = c[1] * first_slot;
  Ciphertext md = (c[2] - c[1]) * first_slot;
  // slot1 = c1, slot2 = c2 via (m1 + (md >> 1)) >> 1 ; but md>>1 puts c2-c1 at slot1,
  // so build: t = m1 + (md>>1) has slot0=c1, slot1=c2-c1 -> use instead direct shifts
  Ciphertext t = (md >> 2) + (m1 >> 1) + (m1 >> 2);
  Ciphertext output = m0 + t;
  output.set_output("output");
}

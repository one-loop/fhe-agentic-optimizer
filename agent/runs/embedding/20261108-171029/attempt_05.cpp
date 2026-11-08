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

  // prefix-difference formulation: out[k] = c0 + (c1-c0)[k>=1] + (c2-c1)[k>=2]
  Ciphertext m0 = c[0] * first_slot;
  Ciphertext m1 = (c[1] - c[0]) * first_slot;
  Ciphertext m2 = (c[2] - c[1]) * first_slot;
  Ciphertext s1 = m1 >> 1;
  Ciphertext s2 = m2 >> 2;
  Ciphertext output = m0 + s1 + s2 + (m1 >> 2) + (m0 >> 1) + (m0 >> 2);
  output.set_output("output");
}

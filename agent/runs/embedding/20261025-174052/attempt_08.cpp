void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  auto reduce = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };

  Ciphertext p0 = indicator * columns[0];
  Ciphertext p1 = indicator * columns[1];
  Ciphertext p2 = indicator * columns[2];
  Ciphertext v = reduce(p2);
  Ciphertext u = reduce(p1 + p2);
  Ciphertext s = reduce(p0 + p1 + p2);

  Ciphertext m0 = (s - u) * first_slot;
  Ciphertext m1 = (u - v) * first_slot;
  Ciphertext m2 = v * first_slot;
  Ciphertext y = (m2 >> 1) + m1;
  Ciphertext output = (y >> 1) + m0;
  output.set_output("output");
}

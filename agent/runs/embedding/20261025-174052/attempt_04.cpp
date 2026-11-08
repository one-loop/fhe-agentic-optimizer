void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  Ciphertext p0 = indicator * columns[0];
  Ciphertext p1 = indicator * columns[1];
  Ciphertext p2 = indicator * columns[2];
  Ciphertext s = p0 + p1 + p2;
  auto reduce = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };
  Ciphertext t0 = reduce(p0);
  Ciphertext t1 = reduce(p1);
  Ciphertext ts = reduce(s);
  Ciphertext d2 = ts - t0 - t1;

  Ciphertext m0 = t0 * first_slot;
  Ciphertext m1 = t1 * first_slot;
  Ciphertext m2 = d2 * first_slot;
  Ciphertext y = (m2 >> 1) + m1;
  Ciphertext output = (y >> 1) + m0;
  output.set_output("output");
}

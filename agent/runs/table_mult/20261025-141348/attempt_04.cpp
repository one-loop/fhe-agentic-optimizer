void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  Ciphertext p0 = indicator * columns[0];
  Ciphertext p1 = indicator * columns[1];
  Ciphertext p2 = indicator * columns[2];
  Ciphertext t = p0 + p1;
  auto red = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };
  Ciphertext s0 = red(p0);
  Ciphertext s2 = red(p2);
  Ciphertext st = red(t);
  Ciphertext s1 = st - s0;
  s0.set_output("output_0");
  s1.set_output("output_1");
  s2.set_output("output_2");
}

void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  Ciphertext r0 = indicator * columns[0];
  Ciphertext r1 = indicator * columns[1];
  Ciphertext r2 = indicator * columns[2];
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    r0 += r0 << static_cast<int>(steps);
    r1 += r1 << static_cast<int>(steps);
    r2 += r2 << static_cast<int>(steps);
  }
  r0.set_output("output_0");
  r1.set_output("output_1");
  r2.set_output("output_2");
}

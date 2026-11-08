void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  Ciphertext r0 = indicator * columns[0];
  Ciphertext r1 = indicator * columns[1];
  Ciphertext r2 = indicator * columns[2];
  Ciphertext a = r0 + r1 + r2;
  Ciphertext b = r1;
  Ciphertext c = r2;
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
  {
    a += a << static_cast<int>(steps);
    b += b << static_cast<int>(steps);
    c += c << static_cast<int>(steps);
  }
  Ciphertext o0 = a - b - c;
  o0.set_output("output_0");
  b.set_output("output_1");
  c.set_output("output_2");
}

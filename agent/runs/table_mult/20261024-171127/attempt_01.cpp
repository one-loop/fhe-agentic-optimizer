void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
    r.push_back(indicator * columns[i]);
  for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
    for (int i = 0; i < 3; ++i)
      r[i] += r[i] << static_cast<int>(steps);
  for (int i = 0; i < 3; ++i)
    r[i].set_output("output_" + to_string(i));
}

void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  vector<Ciphertext> rs;
  for (size_t i = 0; i < columns.size(); ++i)
    rs.push_back(indicator * columns[i]);
  for (size_t steps = 1; steps < (size_t)slots(); steps <<= 1)
    for (size_t i = 0; i < rs.size(); ++i)
      rs[i] += rs[i] << static_cast<int>(steps);
  for (size_t i = 0; i < rs.size(); ++i)
    rs[i].set_output("output_" + to_string(i));
}

void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  vector<Ciphertext> rs;
  for (size_t i = 0; i < columns.size(); ++i)
    rs.push_back(indicator * columns[i]);
  for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
    for (size_t i = 0; i < rs.size(); ++i)
    {
      Ciphertext t = rs[i] << static_cast<int>(steps);
      rs[i] = rs[i] + t;
    }
  for (size_t i = 0; i < rs.size(); ++i)
    rs[i].set_output("output_" + to_string(i));
}

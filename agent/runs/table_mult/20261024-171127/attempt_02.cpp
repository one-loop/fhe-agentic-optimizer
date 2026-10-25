void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int n = static_cast<int>(slots());
  vector<Ciphertext> rot;
  vector<int> shifts = {0, n / 4, n / 2, 3 * n / 4};
  for (int s : shifts)
  {
    if (s == 0)
      rot.push_back(indicator);
    else
      rot.push_back(indicator << s);
  }
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = rot[0] * columns[i];
    for (size_t j = 1; j < shifts.size(); ++j)
      r += rot[j] * (columns[i] << shifts[j]);
    for (int steps = n / 8; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}

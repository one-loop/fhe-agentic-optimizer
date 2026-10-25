void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int n = static_cast<int>(slots());
  int q = n / 4;
  vector<Ciphertext> rot;
  for (int k = 0; k < 4; ++k)
    rot.push_back(k == 0 ? indicator : (indicator << (k * q)));
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Ciphertext r = rot[0] * columns[i];
    for (int k = 1; k < 4; ++k)
      r += rot[k] * (columns[i] << (k * q));
    for (int steps = q >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}

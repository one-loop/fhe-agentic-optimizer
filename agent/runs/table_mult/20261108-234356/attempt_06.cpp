void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));
  int n = slots();
  int half = n >> 1;
  Ciphertext ir = indicator << half;
  for (size_t i = 0; i < columns.size(); ++i)
  {
    Plaintext tr = columns[i] << half;
    Ciphertext r = indicator * columns[i] + ir * tr;
    for (int steps = half >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
